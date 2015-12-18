import re, random, string, subprocess, os, shutil, tempfile
from datetime import date, datetime

resources_path = "./resources"

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
      print index
      if len(index) == 0:
        return "".join( ["{"] + map( lambda idx: "\"" + array[idx] + "\"" + ( ", " if idx < len(array)-1 else ""), xrange(0,len(array)) ) + ["}"] )
      if len(index) == 1:
        return "".join( ["{"] + map( lambda idx: "\"" + array[idx][index[0]] + "\"" + ( ", " if idx < len(array)-1 else ""), xrange(0,len(array)) ) + ["}"] )

    lower_bound_string = "string lower[{0}] = {1};".format( len(self.iterators), write_array(self.bounds, 0 ) )
    upper_bound_string = "string upper[{0}] = {1};".format( len(self.iterators), write_array(self.bounds, 1 ) )
    are_there_symbolics = len(self.symbols) > 0
    print "here"
    symbolic_string = "string symbolics[{0}] = {1};".format( len(self.symbols), write_array( self.symbols ) ) if are_there_symbolics else "// No Symbolics"
    chain_append_string = "{0}.append( LoopNest( RectangularDomain({1}) ) );".format( chain_name, "".join( ["lower, upper, {0}".format(len(self.iterators)), (", symbolics, {0}".format(len(self.symbols))) if are_there_symbolics else "" ] ) )

    return "\t" + "\n\t".join( [lower_bound_string, upper_bound_string, symbolic_string, chain_append_string] )

class DependencySpecification:
  def __init__(self, dependency_list):
    self.dependencies = dependency_list

  def generate_code(self):
    first_statement_regex = re.compile( r"\{\s*(?P<statement>[a-zA-Z]+\w*)?\s*\[" )
    second_statement_regex = re.compile( r"\]\s*\-\>\s*(?P<statement>[a-zA-Z]+\w*)?\s*\[" )

    def transform_dependency_statement( statement ):
      statement = first_statement_regex.sub( "{{ {0}[ ".format("GRAPH_MACRO"), statement)
      statement = second_statement_regex.sub( ",", statement )
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

  def __str__( self ):
    string = "Chain:\n"
    for (nest,loop) in zip(self.chain, xrange(len(self.chain))):
      string += "loop: {0}\n".format( loop )
      for d in xrange(nest.dimensions):
        string += "\t{0}: {1} in {2}..{3}\n".format(d, nest.iterators[d], nest.bounds[d][0], nest.bounds[d][1] )
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
    nest_texts = map( lambda nest: "{{\n{0}\n\t}}".format( nest.generate_code(loop_chain_name) ), self.chain )
    scheduleing_texts = [ "DefaultSequentialSchedule schedule({0});".format( loop_chain_name ), "writeScheduledCode(schedule, \"some_output.cpp\");" ]
    injection_text = "\t" + "\n\t".join( loop_chain_decl_text + nest_texts + scheduleing_texts )

    with open( self.path + "/codegen_template.cpp", 'r' ) as file:
      full_text = file.read()

    full_text = full_text.replace( "GENERATED_GENERATOR_CODE_STAMP", "\n" + injection_text )
    full_text = full_text.replace( "GENERATED_MAIN_CODE_STAMP", "generateChainCode();" )

    print full_text

    with open( self.path + "/code_generator.cpp", 'w' ) as file:
      file.write( full_text )

  def dependency_code_generator_build( self ):
    pass

  def chain_code_generator_build( self ):
    pass

  def dependency_code_generator_run( self ):
    pass

  def chain_code_generator_run( self ):
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

    self.dependency_code_generator_generation()
    self.chain_code_generator_generation( )
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
  dependencies = DependencySpecification(dep_list)

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

  return RegressionTest( test_name, chain_spec, dependencies, schedule_list )

if __name__ == "__main__":
  test_files = ["./work_in_progress.test"]
  tester = Tester( map( parse_test_file, test_files ) )
  tester.run()
