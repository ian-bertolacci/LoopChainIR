import sys, re, random, string, subprocess, os, shutil, tempfile
from datetime import date, datetime

resources_path = "./resources"
FNULL = open(os.devnull, 'w')

class TestFailureException( Exception ):
  def __init__(self, message):
    self.message = message


  def __str__(self):
    return reptr( self.message )



class TestUserMalformedException( Exception ):
  def __init__(self, message):
    self.message = message


  def __str__(self):
    return reptr( self.message )



class TestDevMalformedException( Exception ):
  def __init__(self, message):
    self.message = message


  def __str__(self):
    return reptr( self.message )



class DirectoryStack:
  def __init__( self ):
    self.stack = []


  def pushd( self, path ):
    self.stack.append( os.getcwd() )
    os.chdir( path )


  def popd( self ):
    if len( self.stack ) > 0:
      os.chdir( self.stack.pop() )
    else:
      print "Directory stack empty"



class NestSpecification:
  def __init__(self, list_of_iterators, list_of_bound_pairs, list_of_symbols ):
    self.iterators = list_of_iterators
    self.bounds = list_of_bound_pairs
    self.symbols = list_of_symbols
    self.dimensions = len( list_of_iterators )


  def generate_code( self, chain_name = "chain" ):

    def write_array( array, index=[] ):
      if not isinstance( index, list ):
        index=[index]

      if len(index) == 0:
        return "".join( ["{"] + map( lambda idx: "\"" + array[idx] + "\"" + ( ", " if idx < len(array)-1 else ""), xrange(0,len(array)) ) + ["}"] )
      if len(index) == 1:
        return "".join( ["{"] + map( lambda idx: "\"" + array[idx][index[0]] + "\"" + ( ", " if idx < len(array)-1 else ""), xrange(0,len(array)) ) + ["}"] )

    lower_bound_string = "string lower[{0}] = {1};".format( len(self.iterators), write_array(self.bounds, 0 ) )
    upper_bound_string = "string upper[{0}] = {1};".format( len(self.iterators), write_array(self.bounds, 1 ) )
    are_there_symbolics = len(self.symbols) > 0
    symbolic_string = "string symbolics[{0}] = {1};".format( len(self.symbols), write_array( self.symbols ) ) if are_there_symbolics else "// No Symbolics"
    chain_append_string = "{0}.append( LoopNest( RectangularDomain({1}) ) );".format( chain_name, "".join( ["lower, upper, {0}".format(len(self.iterators)), (", symbolics, {0}".format(len(self.symbols))) if are_there_symbolics else "" ] ) )

    return "  " + "\n  ".join( [lower_bound_string, upper_bound_string, symbolic_string, chain_append_string] )

  def __str__( self ):
    return "({0}){{{1}}}".format( ",".join(self.iterators), ",".join( map( lambda bound:bound[0]+".."+bound[1], self.bounds) ) )

class DependencySpecification:
  def __init__(self, dependency_list):
    self.dependencies = dependency_list


  def generate_code(self):
    first_statement_rx = re.compile( r"\{\s*(?P<statement>[a-zA-Z]+\w*)?\s*\[" )
    second_statement_rx = re.compile( r"\]\s*\-\>\s*(?P<statement>[a-zA-Z]+\w*)?\s*\[" )

    def transform_dependency_statement( statement ):
      statement = first_statement_rx.sub( "{{ {0}[ ".format("GRAPH_MACRO"), statement)
      statement = second_statement_rx.sub( ",", statement )
      return statement

    graph_statements = map( transform_dependency_statement, self.dependencies )

    code = "\n".join( map( lambda a: "expressions.push_back( \"{0}\" );".format(a), graph_statements ) )

    return code



class RegressionTest:
  def __init__( self, name, chain_of_nests, depedency_specification, schedule_list ):
    self.name = name
    self.chain = chain_of_nests
    self.schedules = schedule_list
    self.dependencies = depedency_specification
    self.dirstack = DirectoryStack()


  def __str__( self ):
    string = "Chain:\n"
    for (nest,loop) in zip(self.chain, xrange(len(self.chain))):
      string += "loop: {0}\n".format( loop )
      for d in xrange(nest.dimensions):
        string += "  {0}: {1} in {2}..{3}\n".format(d, nest.iterators[d], nest.bounds[d][0], nest.bounds[d][1] )
    string += "\nDependency:\n" + "".join(self.dependencies) + "\n"
    string += "\nTransformations:\n" + "\n".join(self.schedules)
    return string


  def dependency_code_generator_generation( self ):
    injection_text = self.dependencies.generate_code()

    with open( self.path + "/" + "codegen_template.cpp", 'r' ) as file:
      full_text = file.read()

    full_text = full_text.replace( "GENERATED_GRAPH_CODE_LIST_STAMP", "\n" + injection_text )
    full_text = full_text.replace( "GENERATED_MAIN_CODE_STAMP", "generateGraphCode();" )

    with open( self.path + "/graph_generator.cpp", 'w' ) as file:
      file.write( full_text )


  def chain_code_generator_generation( self ):
    loop_chain_name = "chain";
    loop_chain_decl_text = [ "LoopChain {0};".format( loop_chain_name ) ]
    nest_texts = map( lambda nest: "{{\n{0}\n  }}".format( nest.generate_code(loop_chain_name) ), self.chain )
    scheduleing_texts = [ "DefaultSequentialSchedule schedule({0});".format( loop_chain_name ), "writeScheduledCode(schedule, \"generated_chain_output.cpp\");" ]
    injection_text = "  " + "\n  ".join( loop_chain_decl_text + nest_texts + scheduleing_texts )

    with open( self.path + "/codegen_template.cpp", 'r' ) as file:
      full_text = file.read()

    full_text = full_text.replace( "GENERATED_GENERATOR_CODE_STAMP", "\n" + injection_text )
    full_text = full_text.replace( "GENERATED_MAIN_CODE_STAMP", "generateChainCode();" )

    with open( self.path + "/code_generator.cpp", 'w' ) as file:
      file.write( full_text )


  def dependency_code_generator_build( self ):
    self.dirstack.pushd( self.path )

    exit_code = subprocess.call( ["make graph_generator"], shell=True, stdout=FNULL, stderr=subprocess.STDOUT )

    self.dirstack.popd()

    if exit_code != 0 :
      raise TestFailureException( "Failed to build graph_generator!" )


  def chain_code_generator_build( self ):
    self.dirstack.pushd( self.path )

    exit_code = subprocess.call( ["make code_generator"], shell=True, stdout=FNULL, stderr=subprocess.STDOUT )

    self.dirstack.popd()

    if exit_code != 0 :
      raise TestFailureException( "Failed to build code_generator!" )


  def dependency_code_generator_run( self ):
    self.dirstack.pushd( self.path )

    exit_code = subprocess.call( ["./graph_generator"], shell=True, stdout=FNULL, stderr=subprocess.STDOUT )

    self.dirstack.popd()

    if exit_code != 0 :
      raise TestFailureException( "Failed to run graph_generator!" )


  def chain_code_generator_run( self ):
    self.dirstack.pushd( self.path )

    exit_code = subprocess.call( ["./code_generator"], shell=True, stdout=FNULL, stderr=subprocess.STDOUT )

    self.dirstack.popd()

    if exit_code != 0 :
      raise TestFailureException( "Failed to run code_generator!" )


  def generate_test_code( self ):
    with open( self.path + "/test_template.cpp", 'r' ) as file:
      template_text = file.read()

    self.test_iterators_length = self.determine_iterators_length()

    transformed_text = template_text
    transformed_text = transformed_text.replace( "BOUNDS_CODE_STAMP", self.bounds_generation() )
    transformed_text = transformed_text.replace( "COMPARISON_CODE_STAMP", self.dependency_code_transform( ) )
    transformed_text = transformed_text.replace( "GENERATED_CODE_STAMP", self.chain_code_transform() )
    transformed_text = transformed_text.replace( "TUPLE_TYPE_STAMP", ",".join(["int"]*self.test_iterators_length) )

    with open( self.path + "/test.cpp", 'w' ) as file:
      file.write( transformed_text )


  def determine_iterators_length( self ):
    with open( self.path + "/generated_chain_output.cpp", 'r' ) as file:
      generated_text = file.read()

      max_length = -1

      for stmt_num in xrange( len(self.chain) ):
        stmt_rx = re.compile( r"statement_{0}\((?P<iterators>.*?)\);".format( stmt_num ) )
        max_length = max( max_length, len(stmt_rx.search( generated_text ).group('iterators').split(',')) )

    return 2* max_length+1


  def dependency_code_transform( self ):
    with open( self.path + "/graph_output.cpp", 'r' ) as file:
      generated_text = file.read()

    transformed_text = str( generated_text )
    length_check = -1;
    graph_rx = re.compile( "GRAPH_MACRO\((?P<iterators>.*?)\);");

    for match in graph_rx.finditer( generated_text ):
      iterators = re.split( "\s*,\s*", match.group("iterators") );

      if len(iterators) % 2 != 0 :
        raise TestUserMalformedException( "Odd iterators in graph macro: {0}".format(match.group(0) ) )

      length = len(iterators)/2
      if length_check == -1:
        length_check = length
      elif length != self.test_iterators_length:
        raise TestUserMalformedException( "Provided iterator tuples must conform to the length of the longest iterator tuple! is {0} but should be {1}".format(length, self.test_iterators_length) )
      elif length_check != length:
        raise TestUserMalformedException( "Unequal lengths in previous graph tuple: is {0} but was {1}".format( length, length_check) )

      transformed_text = transformed_text.replace( match.group(0), "graph.connect( make_tuple({0}), make_tuple({1}) );".format( ",".join(iterators[:length]), ",".join(iterators[length:]) ) )

    return transformed_text


  def chain_code_transform( self ):
    with open( self.path + "/generated_chain_output.cpp", 'r' ) as file:
      generated_text = file.read()

    transformed_text = generated_text

    for stmt_num in xrange( len(self.chain) ):
      stmt_rx = re.compile( r"statement_{0}\((?P<iterators>.*?)\);".format( stmt_num ) )
      nest = self.chain[stmt_num]
      depth = len(nest.iterators)

      stmt_iterators = map( lambda a: a.strip(), stmt_rx.search( transformed_text ).group('iterators').split(',') )

      full_iterators = "{0},{1}".format( stmt_num, stmt_iterators[0] )

      for d in xrange( 1, depth ):
        full_iterators += ",0,{0}".format( stmt_iterators[d] )
      full_iterators += ",0"*(self.test_iterators_length-depth*2)

      iteration_text = ("\n" + ("  "*(depth+1))).join(
      [
        "{",
        "auto iter = make_tuple({0});".format( full_iterators ),
        "if( graph.isSatisfied( iter ) ){",
        "  graph.mark( iter );",
        "} else {",
        "  code = -1;",
        "  break;",
        "}",
        "}"
      ]
      )

      generated_text = stmt_rx.sub( iteration_text, generated_text )

    return generated_text


  def bounds_generation( self ):
    symbols = reduce( lambda a,b: a + b,  [ nest.symbols for nest in self.chain] )
    return "\n".join( map( lambda sym: "#define {0} 10".format(sym), symbols ) )


  def build_test( self ):
      self.dirstack.pushd( self.path )

      exit_code = subprocess.call( ["make test"], shell=True, stdout=FNULL, stderr=subprocess.STDOUT )

      self.dirstack.popd()

      if exit_code != 0 :
        raise TestDevMalformedException( "Failed to build test!")


  def run_test( self ):
      self.dirstack.pushd( self.path )

      exit_code = subprocess.call( ["./test"], shell=True, stdout=FNULL, stderr=subprocess.STDOUT )

      self.dirstack.popd()

      if exit_code != 0 :
        raise TestFailureException( "test exited with non-zero exit code! {0}".format( exit_code) )


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

    try:
      self.dependency_code_generator_generation()
      self.chain_code_generator_generation( )

      self.dependency_code_generator_build()
      self.dependency_code_generator_run()

      self.chain_code_generator_build()
      self.chain_code_generator_run()

      self.generate_test_code()

      self.build_test()
      self.run_test()

    finally:
      self.teardown()



class TestRunner:
  def __init__( self, list_of_test_file_names ):
    self.test_file_names = list_of_test_file_names


  def parse_tests( self ):
    print "[Parsing Files]"

    self.tests = []
    for test_file in self.test_file_names:
      print "[{0}]............".format(test_file),

      try:
        self.tests.append( self.parse_test_file(test_file) )
        print "done."
      except TestUserMalformedException as expt:
        print "FAILED!\nTest malformed (user error):\n{0}".format( expt )
      except TestDevMalformedException as expt:
        print "FAILED!\nDeveloperError. Please report:\n{0}".format( expt )

  def run_tests( self ):
    print "[Running Tests]"

    for test in self.tests:
        print "[{0}]............".format(test.name),

        try:
          test.run()
          print "pass."
        except TestFailureException as expt:
          print "FAILED!\n{0}".format( expt )


  def run( self ):
    self.parse_tests()
    self.run_tests()


  def parse_test_file( self, file_name ):
    test_name_rx = re.compile(r"test name\s*:\s*(?P<name>.+)\s*")

    tool_generate_rx = re.compile( r"loop chain\s*:\s*(?P<code>(?:.|\s)+?)\s*:end" )
    tool_loop_nest_rx = re.compile( r"\(\s*(?P<iterators>([a-zA-Z][a-zA-Z0-9]*)((\s*,\s*)([a-zA-Z][a-zA-Z0-9]*))*)\s*\)\s*\{\s*(?P<bounds>(\-?[a-zA-Z0-9]+\s*\.\.\s*\-?[a-zA-Z0-9]+)(\s*,\s*\-?[a-zA-Z0-9]+\s*\.\.\s*\-?[a-zA-Z0-9]+)*)\s*\}")
    tool_iterators_rx = re.compile( r"(?P<iterator>[a-zA-Z][a-zA-Z0-9]*)" )
    tool_bounds_rx = re.compile( r"(?P<lower_bound>\-?[a-zA-Z0-9]+)\.\.(?P<upper_bound>\-?[a-zA-Z0-9]+)" )

    dependency_code_rx = re.compile( r"dependencies\:(?P<code>(?:.|\s)+?)\:end" )
    schedule_rx = re.compile( r"schedule\:(?P<schedules>(?:.|\s)+?)\:end")
    symbol_rx = re.compile( r"[a-zA-Z][a-zA-Z0-9]*" )

    test_file = open( file_name, "r" ).read()

    name_match = test_name_rx.match( test_file )
    if name_match == None:
      raise TestUserMalformedException( "No test name!" )
    else:
      get_name = test_name_rx.match( test_file ).group("name")
      test_name = re.sub( "\s+", "_", test_name_rx.match( test_file ).group("name") )
      if get_name != test_name:
        print "Warning: test name may not contain white-space.\nTest renamed: \"{0}\"".format( test_name )

    # Capture code generator code
    gen_code_groups = tool_generate_rx.findall( test_file )
    # Ensure only one tool code group exists
    if len(gen_code_groups) < 1:
      raise TestUserMalformedException("No loop chain delclarations.")
    elif len(gen_code_groups) > 1:
      raise TestUserMalformedException("Multiple loop chains delclarations.")
    gen_code = gen_code_groups[0]

    # Break apart gen specification
    chain_spec = []
    for nest in tool_loop_nest_rx.finditer( gen_code ):

      iterators_list = tool_iterators_rx.findall( nest.group("iterators") )
      # Map out the bounds into pairs
      bounds_list = map( lambda match: ( match.group("lower_bound"), match.group("upper_bound") ) ,
                 tool_bounds_rx.finditer( nest.group("bounds") ) )

      if len( iterators_list ) != len( bounds_list ):
        raise TestUserMalformedException( "Number of iterators different from number of bounds.")

      symbolics = [ upper for (upper, lower) in bounds_list if symbol_rx.match( upper ) ] + \
            [ lower for (upper, lower) in bounds_list if symbol_rx.match( lower ) ]

      chain_spec.append( NestSpecification( iterators_list, bounds_list, symbolics ) )

    if len( chain_spec ) < 1:
      raise TestUserMalformedException("No loop nests specified.")

    # Capture dependency code
    dep_code_groups = dependency_code_rx.findall( test_file )

    # Ensure only one dependency group exists
    if len(dep_code_groups) < 1:
      raise TestUserMalformedException("No loop dependency delclarations.")
    elif len(dep_code_groups) > 1:
      raise TestUserMalformedException("Multiple dependency delclarations.")

    dep_list = dep_code_groups[0].strip().split("\n")
    if len(dep_list) < 1 or \
       len(dep_list) == 1 and dep_list[0] == "":
      raise TestUserMalformedException( "No dependencies listed" )
    dependencies = DependencySpecification(dep_list)

    # Capture dependency code
    sched_list_groups = schedule_rx.findall( test_file )

    # Ensure only one dependency group exists
    if len(sched_list_groups) < 1:
      raise TestUserMalformedException("No schedule delclarations.")
    elif len(sched_list_groups) > 1:
      raise TestUserMalformedException("Multiple schedule delclarations.")

    schedule_list = sched_list_groups[0].strip().split("\n")

    if len(schedule_list) < 1 or \
       len(schedule_list) == 1 and schedule_list[0] == "":
      raise TestUserMalformedException( "No schedules listed" )

    return RegressionTest( test_name, chain_spec, dependencies, schedule_list )



if __name__ == "__main__":
  if len(sys.argv) <= 1:
    print "usage: python {0} <test file 1> <test file 2> ...".format( sys.argv[0] )
    exit(-1)

  (TestRunner( sys.argv[1:] )).run()
