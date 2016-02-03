//
//  nlprettyprinter.h
//  libminizinc
//
//  Created by Matthew Skelley on 27/01/2016.
//
//

#ifndef __MINIZINC_NLPRETTYPRINTER_HH__
#define __MINIZINC_NLPRETTYPRINTER_HH__

#include <iostream>

#include <minizinc/ast.hh>

namespace MiniZinc {
  
  class NLPrinter {
  private:
    std::ostream& _os;
    
  public:
    NLPrinter(std::ostream& os) : _os(os) {}

    void print(const Model* m);
    
  };
}

#endif