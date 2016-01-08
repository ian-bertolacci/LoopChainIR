import sys, re, random, string, subprocess, os, shutil, tempfile
from datetime import date, datetime

'''
class TestFailureException( Exception ):
  Purpose:
    Exception raised when a RegressionTest fails naturally (not due to a
    malformed test or because of software failure).

  Member Functions:
    __init__( message, log_file = None ):
      Purpose:
        Constructor.

      Parameters:
        message:
          Reason for exception.

        log_file:
          (optional, defaults to None) Log file to write exception message to.

    __str__( ):
      Purpose:
        Function called during str( obj ).
        Produces string representation for printing.

      Returns:
        str of self.message

  Member Variables:
    message:
      Reason for exception.

    log_file:
      Log file to write exception message to. May be None.
'''
class TestFailureException( Exception ):
  def __init__( self, message, log_file = None ):
    self.message = message
    if log_file:
      log_file.write( "TestFailureException: {0}\n".format( message ) )


  def __str__( self ):
    return repr( self.message )



'''
class TestMalformedException( Exception ):
  Purpose:
    Exception raised when a RegressionTest fails because test was broken, but
    it is unknown if malformation was because of user or software error..

  Member Functions:
    __init__( message, log_file = None ):
      Purpose:
        Constructor.

      Parameters:
        message:
          Reason for exception.

        log_file:
          (optional, defaults to None) Log file to write exception message to.

    __str__( ):
      Purpose:
        Function called during str( obj ).
        Produces string representation for printing.

      Returns:
        str of self.message

  Member Variables:
    message:
      Reason for exception.

    log_file:
      Log file to write exception message to. May be None.
'''
class TestMalformedException( Exception ):
  def __init__( self, message, log_file = None ):
    self.message = message
    if log_file:
      log_file.write( "TestUserMalformedException: {0}\n".format( message ) )

  def __str__( self ):
    return repr( self.message )



'''
class TestUserMalformedException( Exception ):
  Purpose:
    Exception raised when a RegressionTest fails because test was broken by the
    user.

  Member Functions:
    __init__( message, log_file = None ):
      Purpose:
        Constructor.

      Parameters:
        message:
          Reason for exception.

        log_file:
          (optional, defaults to None) Log file to write exception message to.

    __str__( ):
      Purpose:
        Function called during str( obj ).
        Produces string representation for printing.

      Returns:
        str of self.message

  Member Variables:
    message:
      Reason for exception.

    log_file:
      Log file to write exception message to. May be None.
'''
class TestUserMalformedException( Exception ):
  def __init__( self, message, log_file = None ):
    self.message = message
    if log_file:
      log_file.write( "TestUserMalformedException: {0}\n".format( message ) )

  def __str__( self ):
    return repr( self.message )



'''
class TestDevMalformedException( Exception ):
  Purpose:
    Exception raised when a RegressionTest fails because test was broken by the
    software.

  Member Functions:
    __init__( message, log_file = None ):
      Purpose:
        Constructor.

      Parameters:
        message:
          Reason for exception.

        log_file:
          (optional, defaults to None) Log file to write exception message to.

    __str__( ):
      Purpose:
        Function called during str( obj ).
        Produces string representation for printing.

      Returns:
        str of self.message

  Member Variables:
    message:
      Reason for exception.

    log_file:
      Log file to write exception message to. May be None.
'''
class TestDevMalformedException( Exception ):
  def __init__( self, message, log_file = None ):
    self.message = message
    if log_file:
      log_file.write( "TestDevMalformedException: {0}\n".format( message ) )

  def __str__( self ):
    return repr( self.message )



'''
class DirectoryStack:
  Purpose:
    Emulate pushd and popd bash commands

  Member Functions:
    __init__( stdout = sys.stdout ):
      Purpose:
        Constructor.

      Parameters:
        stdout:
          (optional, defaults to sys.stdout) file/stream to print stack state
          to.

    pushd( path ):
      Purpose:
        push pwd onto self.stack, chdir to directory specified in path.

      Parameters:
        path:
          full or relative path to directory.

    popd( ):
      Purpose:
        pop a directory from self.stack, chdir to that location.
        If stack is empty, prints error message and does not chdir.

    print_stacK( ):
      Purpose:
        print the current state of the stack to self.stdout

  Member Variables:
    stack:
      list of directories, behaves as stack.

    stdout:
      file/stream to print stack state to.
'''
class DirectoryStack:
  def __init__( self, stdout = sys.stdout ):
    self.stack = []
    self.stdout = stdout

  def pushd( self, path ):
    self.stack.append( os.getcwd() )
    self.print_stack()
    os.chdir( path )


  def popd( self ):
    if len( self.stack ) > 0:
      os.chdir( self.stack.pop() )
      self.print_stack()
    else:
      self.stdout.write( "popd: Directory stack empty\n" );

  def print_stack( self ):
    self.stdout.write( " ".join( self.stack + [os.getcwd()] ) + "\n" )



'''
class NestSpecification:
  Purpose:
    Encapsulates a loop nest and produces LoopChainIR API C++ code.

  Member Functions:
    __init__( ):
      Purpose:
        Constructor.

      Parameters:
        list_of_iterators:
          List of iterator symbols as str-able objects. Typically str objects.

        list_of_bound_pairs:
          List of bound tuples (low, high) for each dimension of the nest as
          str-able ojects. Typically int or str ojbects.

        list_of_symbols:
          List of symbolics present in the bounds list as str-able objects.
          Typically str objects.

    generate_code( chain_name = "chain" ):
      Purpose:
        Generates LoopChainIR API C++ code that generates the specified loop
        nest.

      Returns:
        Genreated code as str-able object. Typically str objects.

      Paremeters:
        chain_name:
          (optional, defaults to "chain" ) string naming the C++ symbol of the
          LoopChain object nest will be appended to.

    __str__( ):
      Purpose:
        Returns the condensed domain representation of the loop nest.

      Returns:
        str object representation of the loop nest.

  Member Variables:
    iterators:
      List of iterator symbols as str-able objects. Typically str objects.

    bounds:
      Lists of bound tuples (low, high) for each dimension of the nest as
      str-able ojects. Typically int() or str ojbects.

    symbols:
      List of symbolics present in the bounds list as str-able objects.
      Typically str objects().

    dimensions:
      depth of loop nest (equivalent to len(iterators) )
'''
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



'''
class DependencySpecification:
  Purpose:
    Encapulates the a test's valid iteration schedule and produces ISL API C
    code pproducing dependency pairs.

  Member Functions:
    __init__( dependency_list ):
      Purpose:
        Constructor.

      Parameters:
        dependency_list:
          list of str-able objects that conform to ISCC representation of a
          dependency relation.
          e.g.: [N]->{[0,i,0]->[1,i,0] : 1 <= i <= N}
          Dependencies must be full-form iterations
          i.e.: [loop nest, iterator, (loop nest, iterator,)* statement]

    generate_code( ):
      Purpose:
        Generates ISL API C code that produces loop nests to produces all pairs
        of dependencies.
        e.g. A dependency [0,i,0]->[1,j,0] will be concatenated to [0,i,0,1,j,0]
        for the purpose of code generation and are broken apart in later code
        transformation passes on the dependency tester code.

      Returns:
        str-able ISL API code that produces loop nests to produces all pairs of
        dependencies.

  Member Variables:
    dependencies:
      list of str-able objects that conform to ISCC representation of a
      dependency relation.
      e.g.: [N]->{[0,i,0]->[1,i,0] : 1 <= i <= N}
      Dependencies must be full-form iterations
      i.e.: [loop nest, iterator, (loop nest, iterator,)* statement]
'''
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



'''
class RegressionTest:
  Purpose:
    Encapulates a loop chains regression test's many representations, and test
    properties.

  Member Functions:
    __init__( name, test_dir, chain_of_nests, depedency_specification,
              schedule_list ):
      Purpose:
        Constructor.

      Parameters:
        name:
          name of the test. Typically str object.

        test_dir:
          path to directory where generated test files and binaries will be
          placed.

        chain_of_nests:
          ordered list of NestSpecification objects representing a loop chain.
          In the chain chain_of_nests[i] is immediately before
          chain_of_nests[i+1].

        depedency_specification:
          DependencySpecification object that specifes the valid iteration order
          for any transformation of the loop chain.

        schedule_list:
          (currently In-Op) ordered list of schedules to transform the loop
          chain by.

    __str__( ):
      Purpose:
        print out terse representation of the list in a simmilar manner to the
        original text of the test.

      Returns:
        str object representing the regression test.

  Member Variables:
    name:
      name of the test. Typically str object.

    chain:
      ordered list of NestSpecification objects representing a loop chain.
      In the chain chain_of_nests[i] is immediately before chain_of_nests[i+1].

    directory:
      directory where test file producing this RegressionTest lives.

    schedules:
      (currently In-Op) ordered list of schedules to transform the loop chain
      by.

    dependencies:
      DependencySpecification object that specifes the valid iteration order for
      any transformation of the loop chain.

    log:
      log file for the test.

    dirstack:
      DirectoryStack object for the test, given self.log as stdout

'''
class RegressionTest:
  def __init__( self, name, test_dir, chain_of_nests, depedency_specification, schedule_list ):
    self.name = name
    self.chain = chain_of_nests
    self.directory = test_dir
    self.schedules = schedule_list
    self.dependencies = depedency_specification

  def __str__( self ):
    string = "Chain:\n"
    for (nest,loop) in zip(self.chain, xrange(len(self.chain))):
      string += "loop: {0}\n".format( loop )
      for d in xrange(nest.dimensions):
        string += "  {0}: {1} in {2}..{3}\n".format(d, nest.iterators[d], nest.bounds[d][0], nest.bounds[d][1] )
    string += "\nDependency:\n" + "".join(self.dependencies) + "\n"
    string += "\nTransformations:\n" + "\n".join(self.schedules)
    return string



'''
class RegressionTest:
  Purpose:
    Encapulates a loop chains regression test's many representations, test
    properties, and is the mechanism that runs the many different components of
    the test, including code generation, building, and the running of the test
    itself.

  Member Functions:
    __init__( static_test, resources_path):
      Purpose:
        Constructor.

      Parameters:
        static_test:
          A RegressionTest object.

        resources_path:
          The absolute or relative path (relative to the script execution
          directory), to the resources directory containing the test template
          files Makefile, codegen_template.cpp, and test_template.cpp

    __str__( ):
      Purpose:
        print out terse representation of the list in a simmilar manner to the
        original text of the test.

      Returns:
        str object representing the regression test.

    write_log( message ):
      Purpose:
        Write message to self.log file. File is flushed after write.

    read_log( ):
      Purpose:
        Returns string of the entire log file. File is is same postion as start
        of method call.

      Returns:
        string of the entire log file.

    makefile_path_transformation( ):
      Purpose:
        Replace PROJECT_DIR_PATH_STAMP in the test Makefile to point to the
        projects Makefile (LoopChainIR/Makefile) relative to self.path and
        writes the resulting code to self.path/Makefile.

    dependency_code_generator_generation( ):
      Purpose:
        Produce the dependency code generator generator. Gets generated ISL API
        C code from self.dependencies DependencySpecification object and injects
        it into codegen_template.cpp in place of GENERATED_GRAPH_CODE_LIST_STAMP
        and replaces GENERATED_MAIN_CODE_STAMP with the generateGraphCode() call
        that executes that code, and writes the resulting code to
        self.path/graph_generator.cpp.

    chain_code_generator_generation( ):
      Purpose:
        Produce the LoopChainIR API code generator code. Gets the genreated
        LoopChainIR API C++ code from the NestSpecification objects in
        self.chain and injects it into codegen_template.cpp in place of
        GENERATED_GENERATOR_CODE_STAMP and replaces GENERATED_MAIN_CODE_STAMP
        with the generateChainCode() call that executes that code, and writes
        the resulting code to self.path/chain_generator.cpp.

    dependency_code_generator_build( ):
      Purpose:
        Build the binary graph_generator that produces the dependency testing
        code by calling the test's Makefile for graph_generator.

      Exceptions:
        If build process exits with non-zero status (indicating a failed build)
        a TestFailureException is raised.

    chain_code_generator_build( ):
      Purpose:
        Build the binary chain_generator that produces the loop chain code by
        calling the test's Makefile for chain_generator.

      Exceptions:
        If build process exits with non-zero exit code
        (indicating a failed build) a TestFailureException is raised.

    dependency_code_generator_run( ):
      Purpose:
        Executes graph_generator to generate the dependency testing code, which
        is writen to self.path/graph_output.cpp.

      Exceptions:
        If process exits with non-zero exit code (indicating some failure,
        either of the executable or the system), TestFailureException is raised.

    dependency_code_generator_run( ):
      Purpose:
        Executes chain_generator to generate the loop chain code, which is
        writen to self.path/generated_chain_output.cpp.

      Exceptions:
        If process exits with non-zero exit code (indicating some failure,
        either of the executable or the system), TestFailureException is raised.

    generate_test_code( ):
      Purpose:
        Call code transformation functions on the dependency testing code
        ( dependency_code_transform() ), the loop_chain code
        ( chain_code_transform() ), calls the bounds defintion producer
        ( bounds_generation() ), produces the templated tuple type for the Graph
        object, and injects the resulting code into test_template.cpp in palace
        of COMPARISON_CODE_STAMP, GENERATED_CODE_STAMP, BOUNDS_CODE_STAMP, and
        TUPLE_TYPE_STAMP. The resulting code is writen to self.path/test.cpp.

    determine_iterators_length( ):
      Purpose:
        Determine the maximum length all the full form iteration tuples both to
        specify the templated tuple type and to lengthen any shorter tuples.

      Returns: the maximum length of all iteration tuples as int object.


    dependency_code_transform( ):
      Purpose:
        Transform the generated graph_output.cpp dependency testing code,
        specifically the GRAPH_MACRO macro, into code that connects the two
        iterations in the dependency graph: graph.connect( a, b ), s.t. {a->b}

      Returns: the transformed code as a str object.

    chain_code_transform( ):
      Purpose:
        Transforms the gerenated generated_chain_output.cpp loop chain code into
        code that checks and marks iterations and their dependencies:
        GENERATED_FOR_LOOP_EXPRESSION
        { // Create the loop's scope
        // Create a tuple object from the iteration expression
        auto iter = make_tuple( GENERATED_ITERATION_EXPRESSION );
        // Determine if the iterations dependendcies are satisfied.
        // Iteration's dependencies are satisfied.
        if( graph.isSatisfied( iter ) ){
          // Mark iteration as satisfied.
          graph.mark( iter );
        }
        // Iteration's dependencies are NOT satisfied.
        else {
          // Set exit code to failure
          code = -1;
          // Print offending iteration
          cout << "FAILED: (" << get<0>(iter) << get<1>(iter) << ... << ")"
               << endl;
          // Print the iterations that it is dependent on, and their state.
          cout << Dependencies: << endl;
          for( auto dep : graph.getDependencies( iter ) ){
            // Print the dependency iteration and if it is satisfied.
            cout << (graph.isSatisfied(dep)? "Satisfied" : "UNSATISFIED" )
                 << " (" << get<0>(dep) << get<1>(dep) << ...  << ")\" << endl;
          }
          // Exit program, returning the exit code.
          return code;
        }
        }

      Returns:
        the transformed code as a str object

    bounds_generation( ):
      Purpose:
        Produce #define macros for the symbolic bounds.
        Currently sets each bound to the constant 10

      Returns:
        the generated code as a str object

    build_test( ):
      Purpose:
        Build the binary test that comprises the regression test by calling the
        test's Makefile for test.

      Exceptions:
        If build process exits with non-zero exit code (indicating a failed
        build) a TestDevMalformedException is raised.

    run_test( ):
      Purpose:
        Executes test.

      Exceptions:
        If process exits with non-zero exit code (indicating some failure,
        either of the executable or the system), TestFailureException is raised.

    setup( ):
      Purpose:
        Construct test environment by creating test directory (self.path) and
        moving resource files to that directory. Will preemptively delete
        existing directories of the same name.

    teardown( ):
      Purpose:
        Deconstruct test enviroment by removing test directory (self.path).

    run( ):
      Purpose:
        Main execution process of the regression test. Calls all methods
        necessary for the construction of the test, and calls setup/teardown
        methods for the test environment.
        teardown method is always called, even in cases of exceptions.

  Member Variables:
    name:
      name of the test. Typically str object.

    chain:
      ordered list of NestSpecification objects representing a loop chain.
      In the chain chain_of_nests[i] is immediately before chain_of_nests[i+1].

    directory:
      directory where test file producing this RegressionTest lives.

    schedules:
      (currently In-Op) ordered list of schedules to transform the loop chain
      by.

    dependencies:
      DependencySpecification object that specifes the valid iteration order for
      any transformation of the loop chain.

    log:
      log file for the test.

    dirstack:
      DirectoryStack object for the test, given self.log as stdout

    path:
      the path to the direcory containing the test files and binaries.
      Constructed as self.directory/self.name"_test_dir"

    resources_path:
      The absolute or relative path (relative to the script execution
      directory), to the resources directory containing the test template files
      Makefile, codegen_template.cpp, and test_template.cpp
'''
class ExecutableRegressionTest( RegressionTest ):
  def __init__( self, static_test, resources_path ):
    # Copy members from the static test
    self.name = static_test.name
    self.chain = static_test.chain
    self.directory = static_test.directory
    self.schedules = static_test.schedules
    self.dependencies = static_test.dependencies

    self.resources_path = resources_path

    # Creat runtime/dynamic members
    self.log = tempfile.SpooledTemporaryFile()
    self.dirstack = DirectoryStack( self.log )
    self.path = "{0}/{1}_test_dir".format( self.directory, self.name )


  def __str__( self ):
    string = "Chain:\n"
    for (nest,loop) in zip(self.chain, xrange(len(self.chain))):
      string += "loop: {0}\n".format( loop )
      for d in xrange(nest.dimensions):
        string += "  {0}: {1} in {2}..{3}\n".format(d, nest.iterators[d], nest.bounds[d][0], nest.bounds[d][1] )
    string += "\nDependency:\n" + "".join(self.dependencies) + "\n"
    string += "\nTransformations:\n" + "\n".join(self.schedules)
    return string


  def write_log( self, message ):
    self.log.write( message + "\n" )
    self.log.flush()


  def read_log( self ):
    self.log.seek(0)
    return "".join( [line for line in self.log] )


  def makefile_path_transformation( self ):
    self.write_log( "[Replacing PROJECT_DIR_PATH_STAMP]" )

    with open( self.path + "/Makefile", 'r' ) as file:
      full_text = file.read()

    relative_path = os.path.relpath( ".", self.path )
    full_text = full_text.replace( "PROJECT_DIR_PATH_STAMP",  relative_path)

    self.write_log( "PROJECT_DIR_PATH_STAMP replaced with {0}".format( relative_path ) )

    with open( self.path + "/Makefile", 'w' ) as file:
      file.write( full_text )


  def dependency_code_generator_generation( self ):
    self.write_log( "[Generating Dependency Code Generator]")

    injection_text = self.dependencies.generate_code()

    with open( self.path + "/" + "codegen_template.cpp", 'r' ) as file:
      full_text = file.read()

    full_text = full_text.replace( "GENERATED_GRAPH_CODE_LIST_STAMP", "\n" + injection_text )
    full_text = full_text.replace( "GENERATED_MAIN_CODE_STAMP", "generateGraphCode();" )

    self.write_log( "Generated Code:\n{0}".format( full_text ) )

    with open( self.path + "/graph_generator.cpp", 'w' ) as file:
      file.write( full_text )


  def chain_code_generator_generation( self ):
    self.write_log( "[Generating Chain Code Generator]" )

    loop_chain_name = "chain";
    loop_chain_decl_text = [ "LoopChain {0};".format( loop_chain_name ) ]
    nest_texts = map( lambda nest: "{{\n{0}\n  }}".format( nest.generate_code(loop_chain_name) ), self.chain )
    scheduleing_texts = [ "DefaultSequentialSchedule schedule({0});".format( loop_chain_name ), "writeScheduledCode(schedule, \"generated_chain_output.cpp\");" ]
    injection_text = "  " + "\n  ".join( loop_chain_decl_text + nest_texts + scheduleing_texts )

    with open( self.path + "/codegen_template.cpp", 'r' ) as file:
      full_text = file.read()

    full_text = full_text.replace( "GENERATED_GENERATOR_CODE_STAMP", "\n" + injection_text )
    full_text = full_text.replace( "GENERATED_MAIN_CODE_STAMP", "generateChainCode();" )

    self.write_log( "Generated Code:\n{0}".format( full_text ) )

    with open( self.path + "/code_generator.cpp", 'w' ) as file:
      file.write( full_text )


  def dependency_code_generator_build( self ):
    self.write_log( "[Building Dependency Code Generator]" )

    self.dirstack.pushd( self.path )

    exit_code = subprocess.call( ["make graph_generator"], shell=True, stdout=self.log, stderr=subprocess.STDOUT )

    self.dirstack.popd()

    if exit_code != 0 :
      raise TestFailureException( "Failed to build graph_generator!", self.log )


  def chain_code_generator_build( self ):
    self.write_log( "[Building Chain Code Generator]")

    self.dirstack.pushd( self.path )

    exit_code = subprocess.call( ["make code_generator"], shell=True, stdout=self.log, stderr=subprocess.STDOUT )

    self.dirstack.popd()

    if exit_code != 0 :
      raise TestFailureException( "Failed to build code_generator!", self.log )


  def dependency_code_generator_run( self ):
    self.write_log( "[Running Dependency Code Generator]" )

    self.dirstack.pushd( self.path )

    exit_code = subprocess.call( ["./graph_generator"], shell=True, stdout=self.log, stderr=subprocess.STDOUT )

    self.dirstack.popd()

    if exit_code != 0 :
      raise TestFailureException( "Failed to run graph_generator!", self.log )


  def chain_code_generator_run( self ):
    self.write_log("[Running Chain Code Generator]")

    self.dirstack.pushd( self.path )

    exit_code = subprocess.call( ["./code_generator"], shell=True, stdout=self.log, stderr=subprocess.STDOUT )

    self.dirstack.popd()

    if exit_code != 0 :
      raise TestFailureException( "Failed to run code_generator!", self.log )


  def generate_test_code( self ):
    self.write_log( "[Generating Test Code]" )

    with open( self.path + "/test_template.cpp", 'r' ) as file:
      template_text = file.read()

    self.test_iterators_length = self.determine_iterators_length()

    transformed_text = template_text
    transformed_text = transformed_text.replace( "BOUNDS_CODE_STAMP", self.bounds_generation() )
    transformed_text = transformed_text.replace( "COMPARISON_CODE_STAMP", self.dependency_code_transform( ) )
    transformed_text = transformed_text.replace( "GENERATED_CODE_STAMP", self.chain_code_transform() )
    transformed_text = transformed_text.replace( "TUPLE_TYPE_STAMP", ",".join(["int"]*self.test_iterators_length) )

    self.write_log( "Generated Code:\n{0}".format(transformed_text) )

    with open( self.path + "/test.cpp", 'w' ) as file:
      file.write( transformed_text )


  def determine_iterators_length( self ):
    self.write_log( "[Determining full iterators length]" )

    with open( self.path + "/generated_chain_output.cpp", 'r' ) as file:
      generated_text = file.read()

      max_length = -1

      for stmt_num in xrange( len(self.chain) ):
        stmt_rx = re.compile( r"statement_{0}\((?P<iterators>.*?)\);".format( stmt_num ) )
        max_length = max( max_length, len(stmt_rx.search( generated_text ).group('iterators').split(',')) )

    return 2* max_length+1


  def dependency_code_transform( self ):
    self.write_log( "[Transforming Dependency Code]" )

    with open( self.path + "/graph_output.cpp", 'r' ) as file:
      generated_text = file.read()

    transformed_text = str( generated_text )
    length_check = -1;
    graph_rx = re.compile( "GRAPH_MACRO\((?P<iterators>.*?)\);");

    for match in graph_rx.finditer( generated_text ):
      iterators = re.split( "\s*,\s*", match.group("iterators") );

      if len(iterators) % 2 != 0 :
        raise TestUserMalformedException( "Odd iterators in graph macro: {0}".format(match.group(0) ), self.log )

      length = len(iterators)/2
      if length_check == -1:
        length_check = length
      elif length != self.test_iterators_length:
        raise TestUserMalformedException( "Provided iterator tuples must conform to the length of the longest iterator tuple! is {0} but should be {1}".format(length, self.test_iterators_length), self.log )
      elif length_check != length:
        raise TestUserMalformedException( "Unequal lengths in previous graph tuple: is {0} but was {1}".format( length, length_check), self.log )

      transformed_text = transformed_text.replace( match.group(0), "graph.connect( make_tuple({0}), make_tuple({1}) );".format( ",".join(iterators[:length]), ",".join(iterators[length:]) ) )

    return transformed_text


  def chain_code_transform( self ):
    self.write_log( "[Transforming Chain Code]" )

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

      get_string = lambda tuple: " << \", \" << ".join( map( lambda i: "get<{0}>({1})".format(i, tuple), xrange(self.test_iterators_length) ) )

      iteration_text = ("\n" + ("  "*(depth+1))).join(
      [
        "{",
        "auto iter = make_tuple({0});".format( full_iterators ),
        "if( graph.isSatisfied( iter ) ){",
        "  graph.mark( iter );",
        "} else {",
        "  code = -1;",
        "  cout << \"FAILED: (\" << {0} << \")\" << endl;".format( get_string("iter") ),
        "  cout << \"Dependencies:\" << endl;",
        "  for( auto dep : graph.getDependencies( iter ) ){",
        "    cout << (graph.isSatisfied(dep)? \"Satisfied\" : \"UNSATISFIED\" ) << \" (\" << {0} << \")\" << endl;".format( get_string("dep") ),
        "  }",
        "  return code;",
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
    self.write_log( "[Building Test]" )

    self.dirstack.pushd( self.path )

    exit_code = subprocess.call( ["make test"], shell=True, stdout=self.log, stderr=subprocess.STDOUT )

    self.dirstack.popd()

    if exit_code != 0 :
      raise TestDevMalformedException( "Failed to build test!", self.log )


  def run_test( self ):
    self.write_log( "[Running Test]" )

    self.dirstack.pushd( self.path )

    exit_code = subprocess.call( ["./test"], shell=True, stdout=self.log, stderr=subprocess.STDOUT )

    self.dirstack.popd()

    if exit_code != 0 :
      raise TestFailureException( "Test exited with non-zero exit code! {0}".format( exit_code), self.log )


  def setup( self ):
    files = ["codegen_template.cpp", "Makefile", "test_template.cpp"]

    # preemptively delete old test workspaces
    try:
      self.teardown()
    except OSError:
      pass

    os.mkdir(self.path)

    def create_full_path( path, sep="/" ):
      return lambda file: path + sep + file

    map( shutil.copyfile,
       map(create_full_path(self.resources_path), files ),
       map(create_full_path(self.path), files ),
      )


  def teardown( self ):
    shutil.rmtree( self.path )


  def run( self ):
    self.setup()

    try:
      self.makefile_path_transformation()

      self.dependency_code_generator_generation()
      self.chain_code_generator_generation()

      self.dependency_code_generator_build()
      self.dependency_code_generator_run()

      self.chain_code_generator_build()
      self.chain_code_generator_run()

      self.generate_test_code()

      self.build_test()
      self.run_test()

    finally:
      #self.teardown()
      pass



'''
class TestRunner:
  Purpose:
    Takes a list of test file names, parses them, tests them. Does not halt on
    the failure of one test (or its parsing).

  Member Functions:
    __init__( list_of_test_file_names ):
      Purpose:
        Constructor

      Parameters:
        list_of_test_file_names:
          list of paths that point to test files.

    parse_tests( ):
      Purpose:
        Parse all test files, insert the resulting RegressionTest into
        self.tests, and print pass/fail status of parsing.
        Does not halt when parsing a test fails.

    run_tests( ):
      Purpose:
        Run each RegressionTest in self.tests and print pass/failure status of
        test.
        Does not halt when a test fails.

    run( ):
      Purpose:
        Entry point for users.
        Runs parse_test then run_tests

    parse_test_file( file_name ):
      Purpose:
        Parse test file and produce a RegressionTest object.


  Member Variables:
    test_file_names:
      List of paths to test files.

    tests:
      list of RegressionTest objects produced by parsing test files in
      parse_tests.
'''
class TestRunner:
  def __init__( self, resources_path, list_of_test_file_names ):
    self.resources_path = resources_path
    self.test_file_names = list_of_test_file_names


  def parse_tests( self ):
    print "[Parsing Files]"

    self.tests = []
    for test_file in self.test_file_names:
      print "[{0}]............".format(test_file),

      try:
        self.tests.append( ExecutableRegressionTest( static_test = self.parse_test_file(test_file), resources_path = self.resources_path ) )
        print "done."
      except TestUserMalformedException as excpt:
        print "FAILED!\nTest malformed (user error):\n{0}".format( excpt )
      except TestDevMalformedException as excpt:
        print "FAILED!\nDeveloperError. Please report:\n{0}".format( excpt )

  def run_tests( self ):
    print "[Running Tests]"

    for test in self.tests:
        print "[{0}]............".format(test.name),

        try:
          test.run()
          print "pass."
          #print "Log Output:{0}".format( test.read_log() )
        except TestFailureException as excpt:
          print "FAILED!\n{0}\n\nLog Output:\n{1}\n".format( excpt, test.read_log() )
        except TestDevMalformedException as excpt:
          print "FAILED!\nDeveloper Error. Please report:\n{0}\nLog output:\n{1}".format( excpt, test.read_log() )
        except TestUserMalformedException as excpt:
          print "FAILED!\nTest is malformed. This is a user error:\n{0}\nLog output:\n{1}".format( excpt, test.read_log() )


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

    # Extract test name
    name_match = test_name_rx.match( test_file )
    if name_match == None:
      raise TestUserMalformedException( "No test name!" )
    else:
      get_name = test_name_rx.match( test_file ).group("name")
      test_name = re.sub( "\s+", "_", test_name_rx.match( test_file ).group("name") )
      if get_name != test_name:
        print "Warning: test name may not contain white-space.\nTest renamed: \"{0}\"".format( test_name )

    # Determine test dir
    test_dir = os.path.dirname( file_name )

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

    return RegressionTest( test_name, test_dir, chain_spec, dependencies, schedule_list )



if __name__ == "__main__":
  if len(sys.argv) <= 1:
    print "usage: python {0} <test file 1> <test file 2> ...".format( sys.argv[0] )
    exit(-1)

  resources_path = os.path.dirname( sys.argv[0] ) + "/resources"

  (TestRunner( resources_path = resources_path, list_of_test_file_names = sys.argv[1:] )).run()
