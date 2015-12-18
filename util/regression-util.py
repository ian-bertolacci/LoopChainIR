import re, random, string, subprocess, os, shutil, tempfile
from datetime import date, datetime

resources_path = "./resources"

class NestSpecification:
    def __init__(self, list_of_iterators, list_of_bound_pairs, list_of_symbols ):
        self.iterators = list_of_iterators
        self.bounds = list_of_bound_pairs
        self.symbols = list_of_symbols
        self.dimensions = len( list_of_iterators )

class RegressionTest:
    def __init__( self, name, chain_of_nests, dependency_list, schedule_list ):
        self.name = name
        self.chain = chain_of_nests
        self.schedules = schedule_list
        self.dependencies = dependency_list

    def __str__( self ):
        string = "Chain:\n"
        for (nest,loop) in zip(self.chain, xrange(len(self.chain))):
            string += "loop: {0}\n".format( loop )
            for d in xrange(nest.dimensions):
                string += "\t{0}: {1} in {2}..{3}\n".format(d, nest.iterators[d], nest.bounds[d][0], nest.bounds[d][1] )
        string += "\nDependency:\n" + "".join(self.dependencies) + "\n"
        string += "\nTransformations:\n" + "\n".join(self.schedules)
        return string

    def dependency_generation( self ):
        first_statement_regex = re.compile( r"\{\s*(?P<statement>[a-zA-Z]+\w*)?\s*\[" )
        second_statement_regex = re.compile( r"\]\s*\-\>\s*(?P<statement>[a-zA-Z]+\w*)?\s*\[" )

        def transform_dependency_statement( statement ):
            statement = first_statement_regex.sub( "{0} {1}{2} ".format("{", "GRAPH_MACRO", "["), statement)
            statement = second_statement_regex.sub( ",", statement )
            return statement

        graph_statements = map( transform_dependency_statement, self.dependencies )

        injection_text = "\n".join( map( lambda a: "expressions.push_back( \"{0}\" );".format(a), graph_statements ) )

        with open( self.path + "/" + "codegen_template.cpp", 'r' ) as file:
          full_text = file.read()

        full_text = full_text.replace( "GENERATED_GRAPH_CODE_LIST_STAMP", injection_text )
        full_text = full_text.replace( "GENERATED_MAIN_CODE_STAMP", "generateFromList();" )

        with open( self.path + "/graph_generator.cpp", 'w' ) as file:
          file.write( full_text )

    def code_generator_generation( self ):
        pass

    def code_generator_run( self ):
        pass

    def setup( self ):
        files = ["codegen_template.cpp", "Makefile", "test_template.cpp"]
        self.path = "./{0}_test_dir".format(self.name)

        # preemptively delete old test workspaces
        try:
            self.teardown()
        except OSError:
            pass

        os.mkdir(self.path)

        def create_full_path( path, sep="/" ):
            return lambda file: path + sep + file

        map( shutil.copyfile,
             map(create_full_path(resources_path), files ),
             map(create_full_path(self.path), files ),
            )

    def teardown( self ):
        shutil.rmtree( self.path )

    def run( self ):
        self.setup()

        self.dependency_generation()

        #self.teardown()


class Tester:
    def __init__(self, list_of_tests ):
        self.tests = list_of_tests



    def run(self):
        for test in self.tests:
            test.run()




def parse_test_file( file_name ):
    test_name_regex = re.compile(r"test name\s*:\s*(?P<name>.+)\s*")

    tool_generate_regex = re.compile( r"loop chain\s*:\s*(?P<code>(?:.|\s)+?)\s*:end" )
    tool_loop_nest_regex = re.compile( r"\(\s*(?P<iterators>([a-zA-Z][a-zA-Z0-9]*)((\s*,\s*)([a-zA-Z][a-zA-Z0-9]*))*)\s*\)\s*\{\s*(?P<bounds>(\-?[a-zA-Z0-9]+\s*\.\.\s*\-?[a-zA-Z0-9]+)(\s*,\s*\-?[a-zA-Z0-9]+\s*\.\.\s*\-?[a-zA-Z0-9]+)*)\s*\}")
    tool_iterators_regex = re.compile( r"(?P<iterator>[a-zA-Z][a-zA-Z0-9]*)" )
    tool_bounds_regex = re.compile( r"(?P<lower_bound>\-?[a-zA-Z0-9]+)\.\.(?P<upper_bound>\-?[a-zA-Z0-9]+)" )

    dependency_code_regex = re.compile( r"dependencies\:(?P<code>(?:.|\s)+?)\:end" )
    schedule_regex = re.compile( r"schedule\:(?P<schedules>(?:.|\s)+?)\:end")
    symbol_regex = re.compile( r"[a-zA-Z][a-zA-Z0-9]*" )

    test_file = open( file_name, "r" ).read()

    name_match = test_name_regex.match( test_file )
    if name_match == None:
        raise Exception( "No test name!" )
    else:
        get_name = test_name_regex.match( test_file ).group("name")
        test_name = re.sub( "\s+", "_", test_name_regex.match( test_file ).group("name") )
        if get_name != test_name:
            print "Warning: test name may not contain white-space.\nTest renamed: \"{0}\"".format( test_name )

    # Capture code generator code
    gen_code_groups = tool_generate_regex.findall( test_file )
    # Ensure only one tool code group exists
    if len(gen_code_groups) < 1:
        raise Exception("No loop chain delclarations.")
    elif len(gen_code_groups) > 1:
        raise Exception("Multiple loop chains delclarations.")
    gen_code = gen_code_groups[0]

    # Break apart gen specification
    chain_spec = []
    for nest in tool_loop_nest_regex.finditer( gen_code ):

        iterators_list = tool_iterators_regex.findall( nest.group("iterators") )
        # Map out the bounds into pairs
        bounds_list = map( lambda match: ( match.group("lower_bound"), match.group("upper_bound") ) ,
                           tool_bounds_regex.finditer( nest.group("bounds") ) )

        if len( iterators_list ) != len( bounds_list ):
            raise Exception( "Number of iterators different from number of bounds.")

        symbolics = [ upper for (upper, lower) in bounds_list if symbol_regex.match( upper ) ] + \
                    [ lower for (upper, lower) in bounds_list if symbol_regex.match( lower ) ]

        chain_spec.append( NestSpecification( iterators_list, bounds_list, symbolics ) )

    if len( chain_spec ) < 1:
        raise Exception("No loop nests specified.")

    # Capture dependency code
    dep_code_groups = dependency_code_regex.findall( test_file )

    # Ensure only one dependency group exists
    if len(dep_code_groups) < 1:
        raise Exception("No loop dependency delclarations.")
    elif len(dep_code_groups) > 1:
        raise Exception("Multiple dependency delclarations.")

    dep_list = dep_code_groups[0].strip().split("\n")
    if len(dep_list) < 1 or \
       len(dep_list) == 1 and dep_list[0] == "":
        raise Exception( "No dependencies listed" )


    # Capture dependency code
    sched_list_groups = schedule_regex.findall( test_file )

    # Ensure only one dependency group exists
    if len(sched_list_groups) < 1:
        raise Exception("No schedule delclarations.")
    elif len(sched_list_groups) > 1:
        raise Exception("Multiple schedule delclarations.")

    schedule_list = sched_list_groups[0].strip().split("\n")

    if len(schedule_list) < 1 or \
       len(schedule_list) == 1 and schedule_list[0] == "":
        raise Exception( "No schedules listed" )

    return RegressionTest( test_name, chain_spec, dep_list, schedule_list )

if __name__ == "__main__":
    test_files = ["./work_in_progress.test"]
    tester = Tester( map( parse_test_file, test_files ) )
    tester.run()
