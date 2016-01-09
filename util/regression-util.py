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
    # Store pwd, then cd to path
    self.stack.append( os.getcwd() )
    self.print_stack()
    os.chdir( path )


  def popd( self ):
    if len( self.stack ) > 0:
      # cd to first path on stack
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

    '''
    write_array( array, index = [] ):
      Purpose:
        Help writing C-style array literals of strings (e.g. { "1", "N" }).
        Two modes:
        1) if index is None, then array is single dimension and is
           indexed from the mapping function only. (i.e. array[idx] )
        2) if index is something else (preferable something that can index/map
           into array) then array is multi-dimensional and array is indesed
           first with the mapping function, then by the provided index
           (i.e. array[idx][index])

      Returns:
        A C-style array literals of strings (e.g. { "1", "N" }).

      Parameters:
        array:
          the addressable set of of elements or singly nested elements to be
          written into an array declaration.

        index:
          (optional, defaults to None) The static index/key into the nested set
          of elements.
          For example if array is a list of lists, then index indexes into the
          nested list.
    '''
    def write_array( array, index=None ):
      values = array if index == None else [ v[index] for v in array ]
      return "{{ {0} }}".format( ", ".join( map( lambda value: "\"{0}\"".format( value ), values ) ) )

    # Generate upper and lower bound array declarations
    lower_bound_string = "string lower[{0}] = {1};".format( len(self.iterators), write_array(self.bounds, 0 ) )
    upper_bound_string = "string upper[{0}] = {1};".format( len(self.iterators), write_array(self.bounds, 1 ) )

    # Generate symbolics array delcaration.
    # in the case that there are no symbolic, no symbolics array is generated.
    are_there_symbolics = len( self.symbols ) > 0
    symbolic_string = "string symbolics[{0}] = {1};".format( len(self.symbols), write_array( self.symbols ) ) if are_there_symbolics else "// No Symbolics"

    # Generate the chain.append(...) statement
    # If there are no symbolics, no symbolics array is used.
    chain_append_string = "{0}.append( LoopNest( RectangularDomain({1}) ) );".format( chain_name, ", ".join( ["lower", "upper", str( len(self.iterators) )] + ( [" symbolics", str( len(self.symbols)) ] if are_there_symbolics else []  ) ) )

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
    # Regex for the first component of the dependency statement:
    # [N,M]->{ captures([1,2,3...]) -> [1',2',3',...] : 1 <= 1}
    first_statement_rx = re.compile( r"\{\s*(?P<statement>[a-zA-Z]+\w*)?\s*\[" )

    # Regex for the second component of the dependency statement:
    # [N,M]->{ [1,2,3...] -> captures([1',2',3',...]) : 1 <= 1}
    second_statement_rx = re.compile( r"\]\s*\-\>\s*(?P<statement>[a-zA-Z]+\w*)?\s*\[" )


    '''
    transform_dependency_statement( statemtn ):
      Purpose:
        transform the given dependency expression
        (e.g. [N]->{ [0,i,0] - > [1,j,0] : 1 <= i,j <= N} )
        into a loop-based set producer expression
        (e.g. [N]->{ GRAPH_MACRO[0,i,0,1,j,0] : 1 <= i,j <= N} )

      Returns:
        Transformed statement

      Parameters:
        statement:
          An ISCC style iteration dependency string.
    '''
    def transform_dependency_statement( statement ):
      # Find and transform the first component
      statement = first_statement_rx.sub( "{{ {0}[ ".format("GRAPH_MACRO"), statement)
      # Find and transform the second component
      statement = second_statement_rx.sub( ",", statement )
      return statement

    # Apply transformation to all dependency strings
    graph_statements = map( transform_dependency_statement, self.dependencies )

    # Generated code to append all statements together in a C++ vector
    # which will iteratively generate each expression.
    # See codegen_template.cpp::generateGraphCode()
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
class ExecutableRegressionTest:
  Purpose:
    Encapulates a loop chains regression test's many representations, and test
    properties much like RegressionTest
    However this also incorperates runtime properties (such as the path to test
    resources) and importantly is the mechanism that runs the many different
    components of the test, including code generation, building, and the running
    of the test itself.

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
        Construct test environment by creating test directory (self.path).
        Will preemptively delete existing directories of the same path.

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
      Name of the test. Typically str object.

    chain:
      Ordered list of NestSpecification objects representing a loop chain.
      In the chain chain_of_nests[i] is immediately before chain_of_nests[i+1].

    directory:
      Directory where test file producing this RegressionTest lives.

    schedules:
      (currently In-Op) Ordered list of schedules to transform the loop chain
      by.

    dependencies:
      DependencySpecification object that specifes the valid iteration order for
      any transformation of the loop chain.

    log:
      Log file for the test.

    dirstack:
      DirectoryStack object for the test, given self.log as stdout

    path:
      The path to the direcory containing the test files and binaries.
      Constructed as self.directory/self.name"_test_dir"

    resources_path:
      The absolute or relative path (relative to the script execution
      directory), to the resources directory containing the test template files
      Makefile, codegen_template.cpp, and test_template.cpp

    test_iterators_length:
      The maximum length of the full form iterations.
      Set in generate_test_code()
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

    with open( self.resources_path + "/Makefile", 'r' ) as file:
      full_text = file.read()

    # Assuming pwd (i.e. "./") is the project directory (ps, this is a
    # moderately valid assumption, but I'd like to not make it.) derive a
    # relative path _from_ self.path _to_ "."
    relative_path = os.path.relpath( ".", self.path )

    # Replace PROJECT_DIR_PATH_STAMP in Makefile
    full_text = full_text.replace( "PROJECT_DIR_PATH_STAMP",  relative_path)

    self.write_log( "PROJECT_DIR_PATH_STAMP replaced with {0}".format( relative_path ) )

    with open( self.path + "/Makefile", 'w' ) as file:
      file.write( full_text )


  def dependency_code_generator_generation( self ):
    self.write_log( "[Generating Dependency Code Generator]")

    # Generate the code for the dependnencies code generation.
    injection_text = self.dependencies.generate_code()

    # Read resouces file codegen_template.cpp
    with open( self.resources_path + "/" + "codegen_template.cpp", 'r' ) as file:
      full_text = file.read()

    # Inject the generated code in to codegen_template.cpp in place of
    # GENERATED_GRAPH_CODE_LIST_STAMP and write the executor of the code
    # ( generateGraphCode() ) into main() in place of GENERATED_MAIN_CODE_STAMP
    full_text = full_text.replace( "GENERATED_GRAPH_CODE_LIST_STAMP", "\n" + injection_text )
    full_text = full_text.replace( "GENERATED_MAIN_CODE_STAMP", "generateGraphCode();" )

    self.write_log( "Generated Code:\n{0}".format( full_text ) )

    # Write code to self.path/graph_generator.cpp
    with open( self.path + "/graph_generator.cpp", 'w' ) as file:
      file.write( full_text )


  def chain_code_generator_generation( self ):
    self.write_log( "[Generating Chain Code Generator]" )

    # Set the C++ symbol of the LoopChain object
    loop_chain_name = "chain";

    # Generated the C++ code for the LoopChain object declaration/instantiation
    loop_chain_decl_text = [ "LoopChain {0};".format( loop_chain_name ) ]

    # Get the generated code from the nest expression, and force them into lower
    # scope (avoid variable name collisions)
    nest_texts = map( lambda nest: "{{\n{0}\n  }}".format( nest.generate_code(loop_chain_name) ), self.chain )

    # Generate the C++ code for scheduling the LoopChain object, and for writing
    # that code (that will be written by the LoopChainIR scheduler) to the file
    # self.path/generated_chain_output.cpp
    scheduleing_texts = [ "DefaultSequentialSchedule schedule({0});".format( loop_chain_name ), "writeScheduledCode(schedule, \"generated_chain_output.cpp\");" ]

    # Concatonate the previously generated texts together with pretty formatting
    injection_text = "  " + "\n  ".join( loop_chain_decl_text + nest_texts + scheduleing_texts )

    # Read the resources file codegen_template.cpp
    with open( self.resources_path + "/codegen_template.cpp", 'r' ) as file:
      full_text = file.read()

    # Inject the generated code in place of GENERATED_GENERATOR_CODE_STAMP and
    # write the executor of the code ( generateChainCode() ) into main() in
    # place of GENERATED_MAIN_CODE_STAMP
    full_text = full_text.replace( "GENERATED_GENERATOR_CODE_STAMP", "\n" + injection_text )
    full_text = full_text.replace( "GENERATED_MAIN_CODE_STAMP", "generateChainCode();" )

    self.write_log( "Generated Code:\n{0}".format( full_text ) )

    # Write code to self.path/code_generator.cpp
    with open( self.path + "/code_generator.cpp", 'w' ) as file:
      file.write( full_text )


  def dependency_code_generator_build( self ):
    self.write_log( "[Building Dependency Code Generator]" )

    self.dirstack.pushd( self.path )

    # Call 'make graph_generator' to build the executable graph_generator, which
    # is what generates the dependency testing code
    exit_code = subprocess.call( ["make graph_generator"], shell=True, stdout=self.log, stderr=subprocess.STDOUT )

    self.dirstack.popd()

    # If make failed, error
    # TODO this should be a different Exception.
    # Potentially a TestMalformedException, since it is a malformation of the
    # test but its source is unknown
    if exit_code != 0 :
      raise TestFailureException( "Failed to build graph_generator!", self.log )


  def chain_code_generator_build( self ):
    self.write_log( "[Building Chain Code Generator]")

    self.dirstack.pushd( self.path )

    # Call 'make code_generator' to build the executable code_generator, which
    # is what generates the loop chain code
    exit_code = subprocess.call( ["make code_generator"], shell=True, stdout=self.log, stderr=subprocess.STDOUT )

    self.dirstack.popd()

    # If make failed, error.
    # Note: this is, infact, a TestFailureException because the API has failed.
    # Though it may be possible that there is a test malformation/software error
    if exit_code != 0 :
      raise TestFailureException( "Failed to build code_generator!", self.log )


  def dependency_code_generator_run( self ):
    self.write_log( "[Running Dependency Code Generator]" )

    self.dirstack.pushd( self.path )

    # Call './graph_generator', generating the dependency testing code, and
    # writing it to self.path/graph_output.cpp
    exit_code = subprocess.call( ["./graph_generator"], shell=True, stdout=self.log, stderr=subprocess.STDOUT )

    self.dirstack.popd()

    # If make failed, error
    # TODO this might need to be a different Exception.
    # Potentially a TestMalformedException, since it is a malformation of the
    # test but its source is unknown
    if exit_code != 0 :
      raise TestFailureException( "Failed to run graph_generator!", self.log )


  def chain_code_generator_run( self ):
    self.write_log("[Running Chain Code Generator]")

    self.dirstack.pushd( self.path )

    # Call './code_generator', generating the loop chain code, and
    # writing it to self.path/generated_chain_output.cpp
    exit_code = subprocess.call( ["./code_generator"], shell=True, stdout=self.log, stderr=subprocess.STDOUT )

    self.dirstack.popd()

    # If make failed, error.
    # Note: this is, infact, a TestFailureException because the API has failed.
    # Though it may be possible that there is a test malformation/software error
    if exit_code != 0 :
      raise TestFailureException( "Failed to run code_generator!", self.log )


  def generate_test_code( self ):
    self.write_log( "[Generating Test Code]" )

    # Read the resources file test_template.cpp
    with open( self.resources_path + "/test_template.cpp", 'r' ) as file:
      template_text = file.read()

    # Find the maximum length of the full form iterations.
    self.test_iterators_length = self.determine_iterators_length()

    transformed_text = template_text

    # Generate and write #define macros for the symbolic bounds in place of
    # BOUNDS_CODE_STAMP
    transformed_text = transformed_text.replace( "BOUNDS_CODE_STAMP", self.bounds_generation() )

    # Generate, transform, and write the dependency tester graph code in place
    # of COMPARISON_CODE_STAMP
    transformed_text = transformed_text.replace( "COMPARISON_CODE_STAMP", self.dependency_code_transform( ) )

    # Generate, transform, and write the test loop chain code in place of
    # GENERATED_CODE_STAMP
    transformed_text = transformed_text.replace( "GENERATED_CODE_STAMP", self.chain_code_transform() )

    # Generate and write the template type for the Graph object in place of
    # TUPLE_TYPE_STAMP
    transformed_text = transformed_text.replace( "TUPLE_TYPE_STAMP", ",".join(["int"]*self.test_iterators_length) )

    self.write_log( "Generated Code:\n{0}".format(transformed_text) )

    # Write to self.path/test.cpp
    with open( self.path + "/test.cpp", 'w' ) as file:
      file.write( transformed_text )


  def determine_iterators_length( self ):
    self.write_log( "[Determining full iterators length]" )

    # Read the generated loop chain output from
    # self.path/generated_chain_output.cpp
    with open( self.path + "/generated_chain_output.cpp", 'r' ) as file:
      generated_text = file.read()

    max_length = -1

    # Dynamically find each statement in the code by producing the regular
    # expression for each statement number, then count the iterators by
    # counting the number of elements in the list after a split over the
    # iterators list group by the ','
    # Note: That might be dangerous...
    # For example, if a statment had a comma in it not as part of the "function"
    # call on statement_#(). This could happen if a function call is written
    # into the stamentc call, such as statement_#(i, j, max(i,j) ).
    # In this case there are 3 iterators but a split on them would count 4
    # TODO Determine if this is a real issue and fix it if necessary/out of
    # caution
    for stmt_num in xrange( len(self.chain) ):
      # Dynamically generate regex
      stmt_rx = re.compile( r"statement_{0}\((?P<iterators>.*?)\);".format( stmt_num ) )
      # Search for statement, extract iterators group, split, count and max()
      max_length = max( max_length, len(stmt_rx.search( generated_text ).group('iterators').split(',')) )

    # Because the iterations generated by the LoopChainIR are not full form,
    # they are shorter than the full form iterations. The iterators found are
    # strictly loop iterations, the loop specification portion of the full form
    # is simply twice as big (e.g. [i,j] => [0,i,0,j]), and there is exactly one
    # terminating position for the statement position
    # (e.g. [0,i,0,j] =>[0,i,0,j,0] )
    return 2* max_length+1


  def dependency_code_transform( self ):
    self.write_log( "[Transforming Dependency Code]" )

    # Read the generated dependency testing code from self.path/graph_output.cpp
    with open( self.path + "/graph_output.cpp", 'r' ) as file:
      generated_text = file.read()

    transformed_text = generated_text

    # We are going to ensure that the user provided the correct lenght of full
    # form iterators in their test specification. The lenght of all iterators
    # should 1) all be equal to each other, 2) be equal to
    # self.test_iterators_length, and 3) be even in parity when joined
    length_check = -1;

    # GRAPH_MACRO regex to find the statements being transformed.
    graph_rx = re.compile( "GRAPH_MACRO\((?P<iterators>.*?)\);");

    # Find each GRAPH_MACRO(...) statement
    for match in graph_rx.finditer( generated_text ):
      # Split iterators by ",", including the surrounding white-space, so that
      # it does not end up in the elements of the list
      iterators = re.split( "\s*,\s*", match.group("iterators") );

      # Check parity (point 3 of the length check)
      if len(iterators) % 2 != 0 :
        raise TestUserMalformedException( "Odd iterators in graph macro: {0}".format(match.group(0) ), self.log )

      length = len(iterators)/2

      # First iterator checked
      if length_check == -1:
        length_check = length

      # Failed point 2
      elif length != self.test_iterators_length:
        raise TestUserMalformedException( "Provided iterator tuples must conform to the length of the longest iterator tuple! is {0} but should be {1}".format(length, self.test_iterators_length), self.log )

      # Failed point 1
      elif length_check != length:
        raise TestUserMalformedException( "Unequal lengths in previous graph tuple: is {0} but was {1}".format( length, length_check), self.log )

      # Replace the found macro with graph.connect(...) code.
      # Inserts graph.connect( make_tuple( first_half_tuple ), make_tuple( second_half_tuple ) )
      transformed_text = transformed_text.replace( match.group(0), "graph.connect( make_tuple({0}), make_tuple({1}) );".format( ",".join(iterators[:length]), ",".join(iterators[length:]) ) )

    return transformed_text


  def chain_code_transform( self ):
    self.write_log( "[Transforming Chain Code]" )

    # Read the generated loop chain ouptut from
    # self.path/generated_chain_output.cpp
    with open( self.path + "/generated_chain_output.cpp", 'r' ) as file:
      generated_text = file.read()

    transformed_text = generated_text

    # Find each statement_#(...) statement
    for stmt_num in xrange( len(self.chain) ):
      # Generate regex for that statement_# macro
      stmt_rx = re.compile( r"statement_{0}\((?P<iterators>.*?)\);".format( stmt_num ) )

      depth = len(self.chain[stmt_num].iterators)

      # Find the match in the text
      match = stmt_rx.search( transformed_text )

      # Split iterators by ",", including the surrounding white-space, so that
      # it does not end up in the elements of the list
      stmt_iterators = re.split( "\s*,\s*", match.group('iterators') )

      # full_iterators is the full form iteration expression for the statemt
      # Generate the 0'th loop of the full form iteration (e.g. [#,i])
      full_iterators = "{0},{1}".format( stmt_num, stmt_iterators[0] )

      # Now we generate the next 1..Depth-1 nest components of the full form
      # iteration
      for d in xrange( 1, depth ):
        # The loop number is always zero here since we donot support imperfect
        # nests
        full_iterators += ",0,{0}".format( stmt_iterators[d] )

      # Generate any necessary padding to make the length of the full form
      # iteration as long self.test_iterators_length
      full_iterators += ",0"*(self.test_iterators_length-depth*2)

      '''
      get_string( tuple ):
        Purpose:
          Produce a line of get<#>( tuple ) for ostream output of a tuple
          (e.g. get<0>(iter) << ", "<< get<1> )

        Returns:
          A string for producing ostream ouput of a tuple

        Parameters:
          tuple: the C++ symbol of the tuple object
      '''
      def get_string( tuple ):
        return " << \", \" << ".join( map( lambda i: "get<{0}>({1})".format(i, tuple), xrange(self.test_iterators_length) ) )

      iteration_text = ("\n" + ("  "*(depth+1))).join(
      [
         # Create the loop's scope
        "{",
        # Create a tuple object from the iteration expression
        "auto iter = make_tuple({0});".format( full_iterators ),
        # Determine if the iterations dependendcies are satisfied.
        "if( graph.isSatisfied( iter ) ){",
        # Mark iteration as satisfied.
        "  graph.mark( iter );",
        "}",
        # Iteration's dependencies are NOT satisfied.
        "else {",
        # Set exit code to failure
        "  code = -1;",
        # Print offending iteration
        "  cout << \"FAILED: (\" << {0} << \")\" << endl;".format( get_string("iter") ),
        # Print the iterations that it is dependent on, and their state.
        "  cout << \"Dependencies:\" << endl;",
        "  for( auto dep : graph.getDependencies( iter ) ){",
        # Print the dependency iteration and if it is satisfied.
        "    cout << (graph.isSatisfied(dep)? \"Satisfied\" : \"UNSATISFIED\" ) << \" (\" << {0} << \")\" << endl;".format( get_string("dep") ),
        "  }",
        # Exit program, returning the exit code.
        "  return code;",
        "}",
        "}"
      ]
      )

      # Replace any text matching the current statment_#(...) regex with the
      # generated iteration_text
      transformed_text = stmt_rx.sub( iteration_text, transformed_text )

    return transformed_text


  def bounds_generation( self ):
    # Collate all the symbols from all the chains
    symbols = reduce( lambda a,b: a + b,  [ nest.symbols for nest in self.chain] )
    # For each symbol give it a #define to 10
    # TODO make this dynamic/repeatable/test a range?
    return "\n".join( map( lambda sym: "#define {0} 10".format(sym), symbols ) )


  def build_test( self ):
    self.write_log( "[Building Test]" )

    self.dirstack.pushd( self.path )

    # Call 'make test' to build the executable test, which
    # is the final test of the whole regression test process
    exit_code = subprocess.call( ["make test"], shell=True, stdout=self.log, stderr=subprocess.STDOUT )

    self.dirstack.popd()

    # If make failed, error.
    # Note: this is, infact, a TestDevMalformedException because at this point,
    # if it were a user error, something would probably have gone wrong.
    # (It could still be a user error, but its not a malformation; rather an
    # incorrectly written test)
    if exit_code != 0 :
      raise TestDevMalformedException( "Failed to build test!", self.log )


  def run_test( self ):
    self.write_log( "[Running Test]" )

    self.dirstack.pushd( self.path )

    # Call './test', running the test of the generated loop chain and its
    # transformations. Will print error information if an error occurs, which
    # is captured in the log file self.log
    exit_code = subprocess.call( ["./test"], shell=True, stdout=self.log, stderr=subprocess.STDOUT )

    self.dirstack.popd()

    # If make failed, error.
    # Note: this is, infact, a TestFailureException because the API has failed.
    # Though it may be possible that there is a test malformation/software error
    if exit_code != 0 :
      raise TestFailureException( "Test exited with non-zero exit code! {0}".format( exit_code), self.log )


  def setup( self ):

    # Preemptively delete old test workspaces
    try:
      self.teardown()
    except OSError:
      pass

    # Construct the test directory at self.path
    os.mkdir(self.path)


  def teardown( self ):
    # Delete the test directory at self.path
    shutil.rmtree( self.path )


  def run( self ):
    # Construct test environment
    self.setup()

    try:
      # Create test makefile
      self.makefile_path_transformation()

      # Create graph_generator.cpp
      self.dependency_code_generator_generation()
      # Create code_generator.cpp
      self.chain_code_generator_generation()

      # Build graph_generator
      self.dependency_code_generator_build()
      # Run graph_generator, producing graph_output.cpp
      self.dependency_code_generator_run()

      # Build code_generator
      self.chain_code_generator_build()
      # Run code_generator, producing generated_chain_output.cpp
      self.chain_code_generator_run()

      # Collect both graph_output.cpp and generated_chain_output.cpp, transform
      # it, and inject it into the template, producing test.cpp
      self.generate_test_code()

      # Build test
      self.build_test()
      # Run test
      self.run_test()

    finally:
      # Deconstruct test environemtn
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

    # The list of ExecutableRegressionTest objects that will be executed
    self.tests = []

    # Attempt to parse each file, construct from the static RegressionTest the
    # dynamic ExecutableRegressionTest, and append it to self.tests
    for test_file in self.test_file_names:
      print "[{0}]............".format(test_file),

      try:
        # Attempt to parse and construct test
        self.tests.append( ExecutableRegressionTest( static_test = self.parse_test_file(test_file), resources_path = self.resources_path ) )
        print "done."

      # Could not parse: User malfomred test
      except TestUserMalformedException as excpt:
        print "FAILED!\nTest malformed (user error):\n{0}".format( excpt )

      # Could not parse: Software error
      except TestDevMalformedException as excpt:
        print "FAILED!\nSoftware Error. Please report:\n{0}".format( excpt )


  def run_tests( self ):
    print "[Running Tests]"

    # Run each test
    for test in self.tests:
        print "[{0}]............".format(test.name),

        try:
          test.run()
          print "pass."
          #print "Log Output:{0}".format( test.read_log() )

        # Test Failed: Failed regression test
        except TestFailureException as excpt:
          print "FAILED!\n{0}\n\nLog Output:\n{1}\n".format( excpt, test.read_log() )

        # Test Failed: User malformed test
        except TestUserMalformedException as excpt:
          print "FAILED!\nTest is malformed. This is a user error:\n{0}\nLog output:\n{1}".format( excpt, test.read_log() )

        # Test Failed: Software error
        except TestDevMalformedException as excpt:
          print "FAILED!\nSoftware Error. Please report:\n{0}\nLog output:\n{1}".format( excpt, test.read_log() )


  def run( self ):
    self.parse_tests()
    self.run_tests()


  def parse_test_file( self, file_name ):
    # The regular expression used to parse the file

    '''
    test_name_rx:
      Expression:
        test name\s*:\s*(?P<name>.+)\s*

      Purpose:
        Find and capture the name of the test

      Captures:
        name:
          name of the test
    '''
    test_name_rx = re.compile(r"test name\s*:\s*(?P<name>.+)\s*")

    '''
    tool_generate_rx:
      Expression:
        loop chain\s*:\s*(?P<code>(?:.|\s)+?)\s*:end

      Purpose:
        Find and capture the terse loop chain of domains expression

      Captures:
        code:
          All the loop chain of domains expression code
    '''
    tool_generate_rx = re.compile( r"loop chain\s*:\s*(?P<code>(?:.|\s)+?)\s*:end" )

    '''
    tool_loop_nest_rx:
      Expression:
        \(\s*(?P<iterators>([a-zA-Z][a-zA-Z0-9]*)((\s*,\s*)([a-zA-Z][a-zA-Z0-9]*))*)\s*\)\s*\{\s*(?P<bounds>(\-?[a-zA-Z0-9]+\s*\.\.\s*\-?[a-zA-Z0-9]+)(\s*,\s*\-?[a-zA-Z0-9]+\s*\.\.\s*\-?[a-zA-Z0-9]+)*)\s*\}

      Purpose:
        Parse apart each component of the domain expression in the loop chain

      Captures:
        iterators:
          The tuple of iterators

        bounds:
          The lis of bounds expressions
    '''
    tool_loop_nest_rx = re.compile( r"\(\s*(?P<iterators>([a-zA-Z][a-zA-Z0-9]*)((\s*,\s*)([a-zA-Z][a-zA-Z0-9]*))*)\s*\)\s*\{\s*(?P<bounds>(\-?[a-zA-Z0-9]+\s*\.\.\s*\-?[a-zA-Z0-9]+)(\s*,\s*\-?[a-zA-Z0-9]+\s*\.\.\s*\-?[a-zA-Z0-9]+)*)\s*\}")

    '''
    tool_iterators_rx:
      Expression:
        (?P<iterator>[a-zA-Z][a-zA-Z0-9]*)

      Purpose:
        Parse apart the iterator tuples

      Captures:
        iterator:
          An individual iterator.
    '''
    tool_iterators_rx = re.compile( r"(?P<iterator>[a-zA-Z][a-zA-Z0-9]*)" )

    '''
    tool_bounds_rx:
      Expression:
        (?P<lower_bound>\-?[a-zA-Z0-9]+)\.\.(?P<upper_bound>\-?[a-zA-Z0-9]+)

      Purpose:
        Parse apart the list of bounds expression into ranges

      Captures:
        lower_bound:
          The lower bound of the range

        upper_bound:
          The upper bound of the range
    '''
    tool_bounds_rx = re.compile( r"(?P<lower_bound>\-?[a-zA-Z0-9]+)\.\.(?P<upper_bound>\-?[a-zA-Z0-9]+)" )


    '''
    dependency_code_rx:
      Expression:
        dependencies\:(?P<code>(?:.|\s)+?)\:end

      Purpose:
        Capture the depenency code

      Captures:
        code:
          The dependency testing code ISCC expression
    '''
    dependency_code_rx = re.compile( r"dependencies\:(?P<code>(?:.|\s)+?)\:end" )

    '''
    schedule_rx:
      Expression:
        schedule\:(?P<schedules>(?:.|\s)+?)\:end

      Purpose:
        Capture the scheduling information.

      Captures:
        schedules:
          Ordered list of scheduling functions to apply
    '''
    schedule_rx = re.compile( r"schedule\:(?P<schedules>(?:.|\s)+?)\:end")

    '''
    symbol_rx:
      Expression:
        [a-zA-Z][a-zA-Z0-9]*

      Purpose:
        General regex for identifiers
    '''
    symbol_rx = re.compile( r"[a-zA-Z][a-zA-Z0-9]*" )

    test_file = open( file_name, "r" ).read()

    # Capture test name
    name_match = test_name_rx.match( test_file )

    # Name not speficied, malformed test
    if name_match == None:
      raise TestUserMalformedException( "No test name!" )

    # Captured name
    else:
      get_name = test_name_rx.match( test_file ).group("name")
      # Replace any white-space with the underscore character
      test_name = re.sub( "\s+", "_", test_name_rx.match( test_file ).group("name") )
      # Warn user if name changed.
      # TODO integrate into log
      if get_name != test_name:
        print "Warning: test name may not contain white-space.\nTest renamed: \"{0}\"".format( test_name )

    # Determine the test files parent directory
    test_dir = os.path.dirname( file_name )

    # Capture code generator code
    gen_code_groups = tool_generate_rx.findall( test_file )

    # Ensure exactly one tool code group exists
    if len(gen_code_groups) < 1:
      raise TestUserMalformedException("No loop chain delclarations.")
    elif len(gen_code_groups) > 1:
      raise TestUserMalformedException("Multiple loop chains delclarations.")

    gen_code = gen_code_groups[0]

    # Break apart gen specification
    chain_spec = []

    # For each domain expression
    for nest in tool_loop_nest_rx.finditer( gen_code ):
      # Get the iterators
      iterators_list = tool_iterators_rx.findall( nest.group("iterators") )

      # Transform the bounds into a orderd list of pairs [ (lower, upper), ... ]
      # ordered by the depth of the loop
      bounds_list = map( lambda match: ( match.group("lower_bound"), match.group("upper_bound") ) ,
                 tool_bounds_rx.finditer( nest.group("bounds") ) )

      # Check that there are the same number of iterators as ranges in the
      # domain
      if len( iterators_list ) != len( bounds_list ):
        raise TestUserMalformedException( "Number of iterators different from number of bounds.")

      # Find and collate the symbolic bounds
      symbolics = [ upper for (upper, lower) in bounds_list if symbol_rx.match( upper ) ] + [ lower for (upper, lower) in bounds_list if symbol_rx.match( lower ) ]

      # Create and append the new NestSpecification
      chain_spec.append( NestSpecification( iterators_list, bounds_list, symbolics ) )

    # Chain must have one or more loop nest
    if len( chain_spec ) < 1:
      raise TestUserMalformedException("No loop nests specified.")

    # Capture dependency code
    dep_code_groups = dependency_code_rx.findall( test_file )

    # Ensure exactly one dependency group exists
    if len(dep_code_groups) < 1:
      raise TestUserMalformedException("No loop dependency delclarations.")
    elif len(dep_code_groups) > 1:
      raise TestUserMalformedException("Multiple dependency delclarations.")

    # Split each ISCC expression by newlines
    # TODO can we make this safer? Its possible that expressions will overflow
    # a line for the purpose of readability
    dep_list = dep_code_groups[0].strip().split("\n")

    # Check that at least 1 dependency is listed
    if len(dep_list) < 1 or len(dep_list) == 1 and dep_list[0] == "":
      raise TestUserMalformedException( "No dependencies listed" )
    dependencies = DependencySpecification(dep_list)

    # Capture scheduling list
    sched_list_groups = schedule_rx.findall( test_file )

    # Ensure only one schedule group exists
    if len(sched_list_groups) < 1:
      raise TestUserMalformedException("No schedule delclarations.")
    elif len(sched_list_groups) > 1:
      raise TestUserMalformedException("Multiple schedule delclarations.")

    # Split by newline
    schedule_list = sched_list_groups[0].strip().split("\n")

    # Check at least one dependency is listed
    if len(schedule_list) < 1 or \
       len(schedule_list) == 1 and schedule_list[0] == "":
      raise TestUserMalformedException( "No schedules listed" )

    # Form and return static RegressionTest
    return RegressionTest( test_name, test_dir, chain_spec, dependencies, schedule_list )



if __name__ == "__main__":
  if len(sys.argv) <= 1:
    print "usage: python {0} <test file 1> <test file 2> ...".format( sys.argv[0] )
    exit(-1)

  resources_path = os.path.dirname( sys.argv[0] ) + "/resources"

  (TestRunner( resources_path = resources_path, list_of_test_file_names = sys.argv[1:] )).run()
