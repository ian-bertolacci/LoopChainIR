import re, random, string, subprocess
from datetime import date, datetime

class ToolSpecification:
    def __init__( self, chain_of_nests ):
        self.chain = chain_of_nests

    def __init__( self ):
        self.chain = []

    def append( self, nest ):
        if nest == None:
            raise Exception("Attempting to add a None to the chain")
        self.chain.append( nest )

    def generate_code( self, file_name ):
        loop_chain_name = "chain";
        chain_decl_string = "\tLoopChain {0};\n".format( loop_chain_name )
        list_of_nest_codes = map( lambda nest: "\t{{\n{0}\n\t}}\n\n".format( nest.generate_code(loop_chain_name) ), self.chain )
        schedule_string = "\tDefaultSequentialSchedule schedule({0});\n".format( loop_chain_name );
        file_open_string = "\tFILE* output_file = fopen( \"{0}\", \"w\" );\n".format( file_name )
        generate_code_string = "\tschedule.codegen( output_file );\n"
        close_file_string = "\tfclose( output_file );\n"
        return "".join( [chain_decl_string] + list_of_nest_codes + [ schedule_string, file_open_string, generate_code_string, close_file_string ] )

class NestExpression:
    def __init__( self, list_of_iterators, list_of_iterator_bounds_pairs, list_of_symbolics ):
        self.iterators = list_of_iterators
        self.bounds = list_of_iterator_bounds_pairs
        self.symbols = list_of_symbolics

    def generate_code( self ):
        return self.generate_code( "chain" );

    def generate_code( self, chain_name ):
        lower_bound_string = "\tstring lower[{0}] = {1};\n".format( len(self.iterators), "".join( ["{"] + map( lambda idx: "\"" + self.bounds[idx][0] + "\"" + ( ", " if idx < len(self.bounds)-1 else ""), xrange(0,len(self.bounds)) ) + ["}"] ))
        upper_bound_string = "\tstring upper[{0}] = {1};\n".format( len(self.iterators), "".join( ["{"] + map( lambda idx: "\"" + self.bounds[idx][1] + "\"" + ( ", " if idx < len(self.bounds)-1 else ""), xrange(0,len(self.bounds)) ) + ["}"] ))
        are_there_symbolics = len(self.symbols) > 0
        symbolic_string = "\tstring symbolics[{0}] = {1};\n".format( len(self.symbols), "".join( ["{"] + map( lambda idx: "\"" + self.symbols[idx] + "\"" + ( ", " if idx < len(self.symbols)-1 else ""), xrange(0,len(self.symbols)) ) + ["}"] )) \
                          if are_there_symbolics else "\t// No Symbolics\n";
        chain_append_string = "\t{0}.append( LoopNest( RectangularDomain({1}) ) );".format( chain_name, "".join( ["lower, upper, {0}".format(len(self.iterators)), (", symbolics, {0}".format(len(self.symbols))) if are_there_symbolics else "" ] ) )
        return "".join( [lower_bound_string, upper_bound_string, symbolic_string, chain_append_string] )

class ControlSpecification:
    def __init__( self, control_code ):
        self.code = control_code

class RegressionTest:
    def __init__( self, name, tool_spec, control_spec ):
        self.name = name
        self.tool_spec = tool_spec
        self.control_spec = control_spec

    def generate_makefile( self ):
        makefile_template = open( "Makefile.template", "r").read();
        makefile_output = re.sub( "CODEGEN_BINARY_STAMP", self.codegen_name, makefile_template )
        makefile_output = re.sub( "TEST_BINARY_STAMP", self.test_name, makefile_output )
        makefile_output = re.sub( "OUTPUT_FILE_STAMP", self.codegen_output_name, makefile_output )
        print makefile_output
        makefile_file = open( "Makefile", "w" )
        makefile_file.write( makefile_output )
        makefile_file.close();

    def transform_statements( self, code_string ):
        # Transform statements
        statement_regex = re.compile( r"(?P<statement>statement(?:_\d+)?)\((?P<iterators>.*)\)" )

        for statement in statement_regex.finditer( code_string ):
            iters_to_string = "SSTR( \"(\" << " + re.sub( ",", " << \", \" << ", statement.group("iterators") ) + " << \")\" )"
            code_string = code_string.replace( statement.group(0)+";", "output.push_back(" + iters_to_string + "); // " + statement.group(0) )

        return code_string

    def generate_code_generator( self ):
        # generate code generator code
        code_gen_template = open( "codegen_template.cpp", "r" ).read();
        code_gen_gen_output = re.sub( "GENERATED_CODE_GENERATOR_STAMP", self.tool_spec.generate_code( self.codegen_output_name ), code_gen_template )

        print "+" + code_gen_gen_output
        codegen_file_output = open( self.codegen_name + ".cpp" , 'w' );
        codegen_file_output.write( code_gen_gen_output )
        codegen_file_output.close()

    def generate_generated_code( self ):
        self.generate_code_generator()
        # Generate code for test loops
        subprocess.call( ["make", "tool"] )
        subprocess.call( [self.codegen_name] )

        # Generate test code
        generated_code = open( self.codegen_output_name, "r" ).read()

        generated_code = self.transform_statements( generated_code )

        print "PYTHON SIDE: GENERATED CODE:"
        print generated_code
        print ":GENERATED CODE :PYTHON SIDE\n\n"

        return generated_code

    def generate_control_code( self ):
        control_code =  self.control_spec.code

        control_code = self.transform_statements( control_code )

        print "PYTHON SIDE: CONTROL CODE:"
        print control_code
        print ":CONTROL CODE :PYTHON SIDE\n\n"

        return control_code

    def generate_symbolic_defines( self ):
        sym_count = 1;
        symbolic_defines = ""

        for nest in self.tool_spec.chain:
            for symbolic in nest.symbols:
                symbolic_defines = symbolic_defines + "#define " + symbolic + " " + str(sym_count*10) + "\n"

        return symbolic_defines

    def generate_test_code( self ):
        self.makefile_name = "Makefile"
        self.codegen_name = self.name + "_codegen"
        self.codegen_output_name = self.name + "_gen_output"
        self.test_name = self.name + "_test"

        self.generate_makefile()

        generated_code = self.generate_generated_code()
        control_code = self.generate_control_code()
        symbolic_defines = self.generate_symbolic_defines()

        test_code_output = open( "test_template.cpp", "r" ).read()
        test_code_output = re.sub( "BOUNDS_CODE_STAMP", symbolic_defines, test_code_output )
        test_code_output = re.sub( "GENERATED_CODE_STAMP", generated_code, test_code_output )
        test_code_output = re.sub( "COMPARISON_CODE_STAMP", control_code, test_code_output )

        test_code_file = open( self.test_name + ".cpp", "w" )
        test_code_file.write( test_code_output );
        test_code_file.close()

    def generate_test( self ):
        self.generate_test_code()
        return subprocess.call( [ "make", "test" ] )

    def run_test( self ):
        # first
        exit_code = self.generate_test()
        exit_code = subprocess.call( [ "./" + self.test_name ] )
        print exit_code
        #subprocess.call( ["make", "clean-generated"] )

def parse_test_file( file_name ):
    test_name_regex = re.compile(r"test name\s*:\s*(?P<name>.+)\s*")
    raw_code_regex = re.compile(r"control\s*:\s*(?P<code>(?:.|\s)*?)\s*:end")
    tool_generate_regex = re.compile( r"generate\s*:\s*(?P<code>(?:.|\s)*?)\s*:end" )
    tool_loop_nest_regex = re.compile( r"\(\s*(?P<iterators>([a-zA-Z][a-zA-Z0-9]*)((\s*,\s*)([a-zA-Z][a-zA-Z0-9]*))*)\s*\)\s*\{\s*(?P<bounds>(\-?[a-zA-Z0-9]+\s*\.\.\s*\-?[a-zA-Z0-9]+)(\s*,\s*\-?[a-zA-Z0-9]+\s*\.\.\s*\-?[a-zA-Z0-9]+)*)\s*\}")
    tool_iterators_regex = re.compile( r"(?P<iterator>[a-zA-Z][a-zA-Z0-9]*)" )
    tool_bounds_regex = re.compile( r"(?P<lower_bound>\-?[a-zA-Z0-9]+)\.\.(?P<upper_bound>\-?[a-zA-Z0-9]+)" )
    symbol_regex = re.compile( r"[a-zA-Z][a-zA-Z0-9]*" )

    test_file = open( file_name, "r" ).read()

    # Capture control code
    control_code_groups = raw_code_regex.findall( test_file )
    # Ensure only one control code group exists
    if len(control_code_groups) < 1:
        raise Exception("No control code.")
    elif len(control_code_groups) > 1:
        raise Exception("Multiple control code bodies.")
    control_spec = ControlSpecification( control_code_groups[0] )

    # Capture code generator code
    gen_code_groups = tool_generate_regex.findall( test_file )
    # Ensure only one tool code group exists
    if len(gen_code_groups) < 1:
        raise Exception("No control code.")
    elif len(gen_code_groups) > 1:
        raise Exception("Multiple control code bodies.")
    gen_code = gen_code_groups[0]

    # Break apart gen specification
    tool_spec = ToolSpecification()
    for nest in tool_loop_nest_regex.finditer( gen_code ):

        iterators_list = tool_iterators_regex.findall( nest.group("iterators") )
        # Map out the bounds into pairs
        bounds_list = map( lambda match: ( match.group("lower_bound"), match.group("upper_bound") ) ,
                           tool_bounds_regex.finditer( nest.group("bounds") ) )

        if len( iterators_list ) != len( bounds_list ):
            raise Exception( "Number of iterators different from number of bounds.")

        symbolics = [ upper for (upper, lower) in bounds_list if symbol_regex.match( upper ) ] + \
                    [ lower for (upper, lower) in bounds_list if symbol_regex.match( lower ) ]

        tool_spec.append( NestExpression( iterators_list, bounds_list, symbolics ) )

    name_match = test_name_regex.match( test_file )
    if name_match == None:
        test_name = "Test_{0}_chain-{1}-{2}-{3}".format( len(tool_spec.chain),
                                                         datetime.now().strftime("%H_%M_%S"),
                                                         datetime.now().strftime("%d_%m_%Y"),
                                                         ''.join(random.SystemRandom().choice(string.ascii_uppercase + string.digits) for _ in range(5))  )
        print "Warning: No name given.\n Naming test: \"{0}\"".format( test_name)
    else:
        get_name = test_name_regex.match( test_file ).group("name")
        test_name = re.sub( "\s+", "_", test_name_regex.match( test_file ).group("name") )
        if get_name != test_name:
            print "Warning: test name may not contain white-space.\nTest renamed: \"{0}\"".format( test_name )

    return RegressionTest( test_name, tool_spec, control_spec )

if __name__ == "__main__":
    test = parse_test_file( "wip.test" )

    test.run_test()
