#ifndef WAVEFRONTTRANSFORMATION_HPP
#define WAVEFRONTTRANSFORMATION_HPP

#include "Transformation.hpp"

namespace LoopChainIR {
  class WavefrontTransformation : public Transformation {
    private:

    public:
      WavefrontTransformation( );
      std::vector<std::string> apply( Schedule &schedule );
      std::vector<std::string> apply( Schedule &schedule, Subspace *subspace );
  };
}

#endif
