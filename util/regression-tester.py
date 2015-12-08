import os, re

class RegressionTest:
    def __init__( self, path ):
        path = path.strip()
        if( path[-1] == "/" ):
            path = path[:-1]

        self.path = path
        self.name = os.path.basename( path )

        if( not os.path.isdir(self.path) ):
            raise Exception( self.path + " does not lead to a directory." )

        print self.path
        print self.name

RegressionTest( "./   " )
