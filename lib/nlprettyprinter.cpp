//
//  nlprettyprinter.cpp
//  libminizinc
//
//  Created by Matthew Skelley on 27/01/2016.
//
//

#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <limits>
#include <iomanip>
#include <map>
#include <minizinc/nlprettyprinter.hh>
#include <minizinc/prettyprinter.hh>
#include <minizinc/model.hh>
#include <minizinc/astexception.hh>
#include <minizinc/iter.hh>
#include <minizinc/hash.hh>

namespace MiniZinc {
  

  
  class nlPlainPrinter {
    
  public:
    std::ostream& os;
    nlPlainPrinter(std::ostream& os0) : os(os0) {}
    
    std::vector<Expression*> variableVector;    //vector to store variables
    std::vector<Expression*> constraintVector;  //vector to store constraints
    std::vector<Expression*> varConstraintVector;   //to store variable constraints like relational operators
    std::vector<int> relationalOpConstraint;
    
    std::vector<Expression*> knownVariableVector;
    
    std::vector<int> unknownVariableVector;  //vector to store unknown variables
    
    std::vector<float> rangeVector; //this will store the r section for each constraint
    
    std::string constraintString;   //This string will store the C section string for printing
    
    std::vector<int> jacobianCountVector;
    std::vector<std::vector<int>> jacobianMatrix;
    //        std::vector<std::vector<Expression*>> jacobianLinCoeffMatrix;
    
    std::vector<const SolveI*> objectiveVector;
    std::vector<int> objTypeVector;  //stores the type of objective eg max or min
    
    int nonLinConstraints = 0;
    std::vector<int> nonLinVarsVector;
    
    std::vector<Expression*> linCoeffVector;
    std::vector<Expression*> linVarVector;  //Stores info about the float_lin_eq variable array
    
    //These vectors store the values for the annotation "starting_at"
    std::vector<int> initVarVector; //stores the variable
    std::vector<Expression*> initValVector; //stores the value
    
    
//    void nlp(const Type& type, Expression* e) {
//      std::cout<<"nlp called, type = "<<&type<<", expression = "<<*e<<std::endl;
//      switch (type.ti()) {
//        case Type::TI_PAR: break;
//        case Type::TI_VAR: os << "var ";break;
//      }
//      if (type.ot()==Type::OT_OPTIONAL)
//        //                os << "opt ";
//        if (type.st()==Type::ST_SET)
//          //                os << "set of ";
//          if (e==NULL) {
//            std::cout<<"e == NULL"<<std::endl;
//            switch (type.bt()) {
//              case Type::BT_INT: os << "int"; break;
//              case Type::BT_BOOL: os << "bool"; break;
//              case Type::BT_FLOAT: os << "float"; break;
//              case Type::BT_STRING: os << "string"; break;
//              case Type::BT_ANN: os << "ann"; break;
//              case Type::BT_BOT: os << "bot"; break;
//              case Type::BT_TOP: os << "top"; break;
//              case Type::BT_UNKNOWN: os << "???"; break;
//            }
//          } else {
//            std::cout<<"e = "<<*e<<std::endl;
//            nlp(e);
//          }
//    }
    
    void nlp(const Type& type, Expression* e) {
      switch (type.ti()) {
        case Type::TI_PAR: break;
        case Type::TI_VAR: os << "var "; break;
      }
      if (type.ot()==Type::OT_OPTIONAL)
//        os << "opt ";
      if (type.st()==Type::ST_SET)
//        os << "set of ";
      if (e==NULL) {
        switch (type.bt()) {
          case Type::BT_INT: os << "int"; break;
          case Type::BT_BOOL: os << "bool"; break;
          case Type::BT_FLOAT: os << "float"; break;
          case Type::BT_STRING: os << "string"; break;
          case Type::BT_ANN: os << "ann"; break;
          case Type::BT_BOT: os << "bot"; break;
          case Type::BT_TOP: os << "top"; break;
          case Type::BT_UNKNOWN: os << "???"; break;
        }
      } else {
        nlp(e);
      }
    }
    
    void nlp(const Annotation& ann) {
      for (ExpressionSetIter it = ann.begin(); it != ann.end(); ++it) {
//        os << ":: ";
        nlp(*it);
      }
    }
    
    void nlp(Expression* e) {   //The variables are decleared in this nlp call.
      std::cout<<"\nnlp called, Expression* e = "<<*e<<std::endl;
      if (e==NULL)
        return;
      switch (e->eid()) {
        case Expression::E_INTLIT:
          std::cout<<"E_INTLIT"<<std::endl;
//          os << e->cast<IntLit>()->v();
          break;
        case Expression::E_FLOATLIT:
        {
          std::cout<<"E_FLOATLIT"<<std::endl;
          std::ostringstream oss;
//          oss << std::setprecision(std::numeric_limits<double>::digits10+1);
//          oss << e->cast<FloatLit>()->v();
          if (oss.str().find("e") == std::string::npos && oss.str().find(".") == std::string::npos)
//            oss << ".0";
//            os << oss.str();
            std::cout<<oss.str()<<std::endl;    //added to check what outputs
        }
          break;
        case Expression::E_SETLIT:
        {
          std::cout<<"E_SETLIT"<<std::endl;
          const SetLit& sl = *e->cast<SetLit>();
          if (sl.isv()) {
            if (sl.isv()->size()==0) {
//              os << (_flatZinc ? "1..0" : "{}");
            } else if (sl.isv()->size()==1) {
//              os << sl.isv()->min(0) << ".." << sl.isv()->max(0);
            } else {
              if (!sl.isv()->min(0).isFinite())
//                os << sl.isv()->min(0) << ".." << sl.isv()->max(0) << "++";
//                os << "{";
                for (IntSetRanges isr(sl.isv()); isr();) {
                  if (isr.min().isFinite() && isr.max().isFinite()) {
                    for (IntVal i=isr.min(); i<=isr.max(); i++) {
//                      os << i;
//                      if (i<isr.max())
//                        os << ",";
                    }
                    ++isr;
//                    if (isr())
//                      os << ",";
                  }
                }
//                os << "}";
//                if (!sl.isv()->max(sl.isv()->size()-1).isFinite())
//                  os << "++" << sl.isv()->min(sl.isv()->size()-1) << ".." << sl.isv()->max(sl.isv()->size()-1);
              }
            } else {
//              os << "{";
              for (unsigned int i = 0; i < sl.v().size(); i++) {
                nlp(sl.v()[i]);
//                if (i<sl.v().size()-1)
//                  os << ",";
              }
//              os << "}";
            }
          }
          break;
        case Expression::E_BOOLLIT:
          os << (e->cast<BoolLit>()->v() ? "true" : "false");
//          std::cout<<"E_BOOLLIT"<<std::endl;
          break;
        case Expression::E_STRINGLIT:
          os << "\"" << Printer::escapeStringLit(e->cast<StringLit>()->v()) << "\"";
//          std::cout<<"E_STRINGLIT"<<std::endl;
          break;
        case Expression::E_ID:
        {
          std::cout<<"E_ID"<<std::endl;
          if (e==constants().absent) {
//            os << "<>";
          } else {
            const Id* id = e->cast<Id>();
            if (id->idn() == -1) {
//              os << id->v();
            } else {
//              os << "X_INTRODUCED_" << id->idn();
            }
          }
        }
          break;
        case Expression::E_TIID:
//          os << "$" << e->cast<TIId>()->v();
//          std::cout<<"E_TIID"<<std::endl;
          break;
        case Expression::E_ANON:
//          os << "_";
          break;
        case Expression::E_ARRAYLIT:
        {
//          std::cout<<"E_ARRAYLIT"<<std::endl;
          const ArrayLit& al = *e->cast<ArrayLit>();
          int n = al.dims();
          if (n == 1 && al.min(0) == 1) {
//            os << "[";
            for (unsigned int i = 0; i < al.v().size(); i++) {
              nlp(al.v()[i]);
//              if (i<al.v().size()-1)
//              os << ",";
            }
//            os << "]";
          } else if (n == 2 && al.min(0) == 1 && al.min(1) == 1) {
//            os << "[|";
            for (int i = 0; i < al.max(0); i++) {
              for (int j = 0; j < al.max(1); j++) {
                nlp(al.v()[i * al.max(1) + j]);
//                if (j < al.max(1)-1)
//                  os << ",";
              }
//              if (i<al.max(0)-1)
//                os << "|";
            }
//            os << "|]";
          } else {
//            os << "array" << n << "d(";
            for (int i = 0; i < al.dims(); i++) {
//              os << al.min(i) << ".." << al.max(i);
//              os << ",";
            }
//            os << "[";
            for (unsigned int i = 0; i < al.v().size(); i++) {
              nlp(al.v()[i]);
//              if (i<al.v().size()-1)
//                os << ",";
            }
//            os << "])";
          }
        }
          break;
        case Expression::E_ARRAYACCESS:
        {
//          std::cout<<"E_ARRAYACCESS"<<std::endl;
          const ArrayAccess& aa = *e->cast<ArrayAccess>();
          nlp(aa.v());
//          os << "[";
          for (unsigned int i = 0; i < aa.idx().size(); i++) {
            nlp(aa.idx()[i]);
//            if (i<aa.idx().size()-1)
//              os << ",";
          }
//          os << "]";
        }
          break;
        case Expression::E_COMP:
        {
          std::cout<<"E_COMP"<<std::endl;
          Comprehension& c = *e->cast<Comprehension>();
//          os << (c.set() ? "{" : "[");
          nlp(c.e());
//          os << " | ";
          for (int i=0; i<c.n_generators(); i++) {
            for (int j=0; j<c.n_decls(i); j++) {
//              os << c.decl(i,j)->id()->v();
//              if (j < c.n_decls(i)-1)
//                  os << ",";
            }
//            os << " in ";
            nlp(c.in(i));
//            if (i < c.n_generators())
//                os << ", ";
          }
          if (c.where() != NULL) {
//            os << " where ";
            nlp(c.where());
          }
//          os << (c.set() ? "}" : "]");
        }
          break;
        case Expression::E_ITE:
        {
//          std::cout<<"E_ITE"<<std::endl;
          ITE& ite = *e->cast<ITE>();
          for (int i = 0; i < ite.size(); i++) {
//            os << (i == 0 ? "if " : " elseif ");
            nlp(ite.e_if(i));
//            os << " then ";
            nlp(ite.e_then(i));
          }
//          os << " else ";
          nlp(ite.e_else());
//          os << " endif";
        }
          break;
        case Expression::E_BINOP:
        {
//          std::cout<<"E_BINOP"<<std::endl;
          const BinOp& bo = *e->cast<BinOp>();
//          if (ps & PN_LEFT)
//            os << "(";
          nlp(bo.lhs());
//          if (ps & PN_LEFT)
//              os << ")";
          switch (bo.op()) {
            case BOT_PLUS:
//              os<<"+";
              break;
            case BOT_MINUS:
//              os<<"-";
              break;
            case BOT_MULT:
//              os<<"*";
              break;
            case BOT_DIV:
//              os<<"/";
              break;
            case BOT_IDIV:
//              os<<" div ";
              break;
            case BOT_MOD:
//              os<<" mod ";
              break;
            case BOT_LE:
//              os<<" < ";
              break;
            case BOT_LQ:
//              os<<"<=";
              break;
            case BOT_GR:
//              os<<" > ";
              break;
            case BOT_GQ:
//              os<<">=";
              break;
            case BOT_EQ:
//              os<<"==";
              break;
            case BOT_NQ:
//              os<<"!=";
              break;
            case BOT_IN:
//              os<<" in ";
              break;
            case BOT_SUBSET:
//              os<<" subset ";
              break;
            case BOT_SUPERSET:
//              os<<" superset ";
              break;
            case BOT_UNION:
//              os<<" union ";
              break;
            case BOT_DIFF:
//              os<<" diff ";
              break;
            case BOT_SYMDIFF:
//              os<<" symdiff ";
              break;
            case BOT_INTERSECT:
//              os<<" intersect ";
              break;
            case BOT_PLUSPLUS:
//              os<<"++";
              break;
            case BOT_EQUIV:
//              os<<" <-> ";
              break;
            case BOT_IMPL:
//              os<<" -> ";
              break;
            case BOT_RIMPL:
//              os<<" <- ";
              break;
            case BOT_OR:
//              os<<" \\/ ";
              break;
            case BOT_AND:
//              os<<" /\\ ";
              break;
            case BOT_XOR:
//              os<<" xor ";
              break;
            case BOT_DOTDOT:
//              os<<"..";
              break;
            default:
              assert(false);
              break;
          }
//          if (ps & PN_RIGHT)
//              os << "(";
          nlp(bo.rhs());
//          if (ps & PN_RIGHT)
//              os << ")";
        }
          break;
        case Expression::E_UNOP:
        {
          std::cout<<"E_UNOP"<<std::endl;
          const UnOp& uo = *e->cast<UnOp>();
          switch (uo.op()) {
            case UOT_NOT:
//              os << "not ";
              break;
            case UOT_PLUS:
//              os << "+";
              break;
            case UOT_MINUS:
//              os << "-";
              break;
            default:
              assert(false);
              break;
          }
          bool needParen = (uo.e()->isa<BinOp>() || uo.e()->isa<UnOp>() || !uo.ann().isEmpty());
//          if (needParen)
//              os << "(";
          nlp(uo.e());
//          if (needParen)
//              os << ")";
        }
          break;
        case Expression::E_CALL:
        {
          std::cout<<"E_CALL"<<std::endl;
          const Call& c = *e->cast<Call>();
//          os << c.id() << "(";
          for (unsigned int i = 0; i < c.args().size(); i++) {
            nlp(c.args()[i]);
//            if (i < c.args().size()-1)
//                os << ",";
          }
//          os << ")";
        }
          break;
        case Expression::E_VARDECL:
        {
//          std::cout<<"E_VARDECL = "<<*e<<std::endl;
          VarDecl& vd = *e->cast<VarDecl>();
          
          if (TypeInst* ti = vd.ti()->dyn_cast<TypeInst>()) {
            Expression* e = ti->domain();
            if (!ti->isarray()) {
              //The variable is unknown
              variableVector.push_back(&vd);
              vd.payload(variableVector.size()-1);
              nonLinVarsVector.push_back(0);
              relationalOpConstraint.push_back(-1);
            }
            else {
              //The variable is an array
              knownVariableVector.push_back(&vd);
              vd.payload(-knownVariableVector.size());
            }
          }
          nlp(vd.ann());
          if (vd.e()) {
            nlp(vd.e());
          }
          
        }
          break;
        case Expression::E_LET:
        {
          const Let& l = *e->cast<Let>();
//          os << "let {";
          for (unsigned int i = 0; i < l.let().size(); i++) {
            Expression* li = l.let()[i];
//            if (!li->isa<VarDecl>())
//              os << "constraint ";
            nlp(li);
//            if (i<l.let().size()-1)
//                os << ", ";
            }
//            os << "} in (";
            nlp(l.in());
//            os << ")";
        }
            break;
        case Expression::E_TI:
        {
            std::cout<<"E_TI"<<std::endl;
            std::cout<<"e* = "<<*e<<std::endl;
            const TypeInst& ti = *e->cast<TypeInst>();
            std::cout<<"ti = "<<ti<<std::endl;
            if (ti.isarray()) {
//              std::cout<<"ti.isarray()"<<std::endl;
//              os << "array [";
              for (unsigned int i = 0; i < ti.ranges().size(); i++) {
                nlp(Type::parint(), ti.ranges()[i]);
//                  if (i < ti.ranges().size()-1)
//                    os << ",";
                }
//                os << "] of ";
            }
//            std::cout<<"ti.domain() = "<<*ti.domain()<<std::endl;
            nlp(ti.type(),ti.domain());
        }
      }
      if (!e->isa<VarDecl>()) {
        nlp(e->ann());
      }
      //os << "Epression";
    }

    
    void nlp(const Item* i) {
      if (i==NULL)
        return;
//      if (i->removed())
//          os << "% ";
      switch (i->iid()) {
        case Item::II_INC:
//          os << "include \"" << i->cast<IncludeI>()->f() << "\"";
          break;
        case Item::II_VD:
          nlp(i->cast<VarDeclI>()->e());
//          std::cout<<*i;  //Here is where all the variables are declared
          break;
        case Item::II_ASN:
//          os << i->cast<AssignI>()->id() << " = ";
          nlp(i->cast<AssignI>()->e());
          break;
        case Item::II_CON:
//          os << "constraint ";
          nlp(i->cast<ConstraintI>()->e());
//          std::cout<<*i->cast<ConstraintI>()->e()<<std::endl;
          
          if (Expression* e = i->cast<ConstraintI>()->e()) {
            const Call& c = *e->cast<Call>();
            if (c.id() == "float_le") {
              for (int j = 0; j < c.args().size(); j++) {
                if (Id* ident = c.args()[j]->dyn_cast<Id>()) {
                  relationalOpConstraint[ident->decl()->payload()] = varConstraintVector.size();
                  varConstraintVector.push_back(i->cast<ConstraintI>()->e());
                  break;
                }
              }
            }
            else {
              constraintVector.push_back(i->cast<ConstraintI>()->e());
            }
          }
          break;
        case Item::II_SOL:
        {
          const SolveI* si = i->cast<SolveI>();
//          os << "solve ";
          nlp(si->ann());
          switch (si->st()) {
          case SolveI::ST_SAT:
//              os << " satisfy";
              break;
          case SolveI::ST_MIN:
          objectiveVector.push_back(si);
          objTypeVector.push_back(0);
//              os << " minimize ";
              nlp(si->e());
              break;
          case SolveI::ST_MAX:
          objectiveVector.push_back(si);
          objTypeVector.push_back(1);
//              os << " maximize ";
              nlp(si->e());
              break;
            }
        }
            break;
        case Item::II_OUT:
//          os << "output ";
          nlp(i->cast<OutputI>()->e());
          break;
        case Item::II_FUN:
        {
          const FunctionI& fi = *i->cast<FunctionI>();
          if (fi.ti()->type().isann() && fi.e() == NULL) {
//            os << "annotation ";
          } else if (fi.ti()->type() == Type::parbool()) {
//            os << "test ";
          } else if (fi.ti()->type() == Type::varbool()) {
//            os << "predicate ";
          } else {
//            os << "function ";
            nlp(fi.ti());
//            os << " : ";
          }
//          os << fi.id();
          if (fi.params().size() > 0) {
//            os << "(";
            for (unsigned int i = 0; i < fi.params().size(); i++) {
              nlp(fi.params()[i]);
//              if (i<fi.params().size()-1)
//                  os << ",";
            }
//            os << ")";
          }
          nlp(fi.ann());
          if (fi.e()) {
//            os << " = ";
            nlp(fi.e());
          }
        }
          break;
      }
//      os << ";" << std::endl;
    }
    
    void printHeader() {
      os<<"g3 0 1 0"<<std::endl;
      os<<variableVector.size()<<" "<<constraintVector.size()<<" "<<objectiveVector.size()<<" 0 "<<constraintVector.size()<<std::endl;
      os<<nonLinConstraints<<" 0"<<std::endl;
      os<<"0 0"<<std::endl;
      int nonLinVarsSum = 0;
      for (int i = 0; i < nonLinVarsVector.size(); i++) {
        nonLinVarsSum = nonLinVarsSum + nonLinVarsVector[i];
      }
      os<<nonLinVarsSum<<" 0 0"<<std::endl;
      os<<"0 0 0 1"<<std::endl;
      os<<"0 0 0 0 0"<<std::endl;
      int nonzerosInJacobian = 0;
      for (int i = 0; i < constraintVector.size(); i++) {
        for (int j = 0; j < variableVector.size(); j++) {
          if (jacobianMatrix[i][j] != 0)  {
            nonzerosInJacobian++;
          }
        }
      }
      os<<nonzerosInJacobian<<" "<<objectiveVector.size()<<std::endl;
      os<<"0 0"<<std::endl;
      os<<"0 0 0 0 0"<<std::endl;
    }
    
    //The following prints out the variables
    void printVariables() {
      std::cout<<"\n-----------------------------------------------------------\n"<<std::endl;
      std::cout<<"-- printVaribles()"<<std::endl;
      
      std::ofstream output_txt;
      output_txt.open("/Users/matthewskelley/NICTA/NICTA/FZNtoNL/output_txt.txt");
      
      output_txt<<"HERE"<<std::endl;
      
      std::cout<<&os<<std::endl;
      for (int i = 0; i < variableVector.size(); i++) {
        Annotation& ann = variableVector[i]->ann();
      
        for (ExpressionSetIter it = ann.begin(); it != ann.end(); ++it) {
          if (const Expression* e =*it) {
            if (e->eid() == Expression::E_CALL) {
              const Call& c = *e->dyn_cast<Call>();
              if (c.id() == "starting_at") {
                initValVector.push_back(c.args()[0]);
                initVarVector.push_back(i);
              }
            }
//            if (const Id* id = e->dyn_cast<Id>()) {
//              if (id->v() == "output_var") {
//                output_txt<<id->v()<<" ";
//                VarDecl* vd = variableVector[i]->cast<VarDecl>();
//                output_txt<<vd->id()->v()<<" "<<vd->payload();
//              }
//              else if (id->v() == "output_array") {
//                output_txt<<"ARRAY";
//              }
//              output_txt<<std::endl;
//            }
          }
        }
      }
      
      output_txt.close();
    }
    
    void updateJacobianMatrix_aux() {
      
      for (int i = 0; i < constraintVector.size(); i++) {
        std::cout<<"updateJacobianMatrix #"<<i<<std::endl;
        updateJacobianMatrix(constraintVector[i]);
      }
    }
    
    void updateJacobianMatrix(Expression* e) {
      
      const Call& c = *e->cast<Call>();
      std::vector<int> tmpVector;
      jacobianCountVector.push_back(0);
      
      for (int i = 0; i < variableVector.size(); i++) {
        tmpVector.push_back(0);
      }
      
      std::cout<<*e<<std::endl;
      
      if (c.id() == "float_lin_eq" || c.id() == "float_lin_le") {
        ArrayLit* al = NULL;
        if (ArrayLit* al2 = c.args()[1]->dyn_cast<ArrayLit>()) {
          al = al2;
        } else {
          al = c.args()[1]->dyn_cast<Id>()->decl()->e()->cast<ArrayLit>();
        }
        for (int j = 0; j < al->v().size(); j++) {
          if (tmpVector[al->v()[j]->cast<Id>()->decl()->payload()] == 0) {
            tmpVector[al->v()[j]->cast<Id>()->decl()->payload()] = 1;
            jacobianCountVector[jacobianCountVector.size() - 1]++;
          }
        }
        
//        if (ArrayLit* al = c.args()[1]->dyn_cast<ArrayLit>()) {
//          for (int j = 0; j < al->v().size(); j++) {
//            if (tmpVector[al->v()[j]->cast<Id>()->decl()->payload()] == 0) {
//              tmpVector[al->v()[j]->cast<Id>()->decl()->payload()] = 1;
//              jacobianCountVector[jacobianCountVector.size() - 1]++;
//            }
//          }
//        }
//        else if (c.args()[1]->dyn_cast<Id>()->decl()->payload()<0) {
//          std::cout<<"HERE"<<std::endl;
//          std::cout<<c.args()[1]<<std::endl;
//          if (VarDecl* var = knownVariableVector[-(c.args()[1]->dyn_cast<Id>()->decl()->payload())]->dyn_cast<VarDecl>()) {
//            std::cout<<"HERE"<<std::endl;
//            if (ArrayLit* al = var->e()->dyn_cast<ArrayLit>()) {
//              std::cout<<"HERE"<<std::endl;
//              for (int j = 0; j < al->v().size(); j++) {
//                if (tmpVector[al->v()[j]->cast<Id>()->decl()->payload()] == 0) {
//                  tmpVector[al->v()[j]->cast<Id>()->decl()->payload()] = 1;
//                  jacobianCountVector[jacobianCountVector.size() - 1]++;
//                }
//              }
//            }
//          }
//        }
        if (Id* ident = c.args()[2]->dyn_cast<Id>()) {
          if (ident->decl()->payload() >= 0) {
            if (tmpVector[ident->decl()->payload()] == 0) {
              tmpVector[ident->decl()->payload()] = 1;
              jacobianCountVector[jacobianCountVector.size() - 1]++;
            }
          }
        }
      }
      else {
        nonLinConstraints++;
        for (unsigned int j = 0; j < c.args().size(); j++) {
          if (Id* ident = c.args()[j]->dyn_cast<Id>()) {
            if (ident->decl()->payload() >= 0) {
              if (tmpVector[ident->decl()->payload()] == 0) {
                if (j == c.args().size() - 1) {
                  tmpVector[ident->decl()->payload()] = -1;
                  jacobianCountVector[jacobianCountVector.size() - 1]++;
                }
                else {
                  tmpVector[ident->decl()->payload()] = 1;
                  jacobianCountVector[jacobianCountVector.size() - 1]++;
                  if (nonLinVarsVector[ident->decl()->payload()] == 0) {
                    nonLinVarsVector[ident->decl()->payload()] = 1;
                  }
                }
              }
            }
          }
        }
      }
      jacobianMatrix.push_back(tmpVector);
    }
    
    //The following prints out the C section of the .nl file
    void printConstraints() {
      std::cout<<"\n-----------------------------------------------------------\n"<<std::endl;
      std::cout<<"-- printConstraints()"<<std::endl;
      //It loops through all of the constraints in the constraint vector we created
      for (int i = 0; i < constraintVector.size(); i++) {
        std::cout<<"C"<<i<<std::endl;
        os<<"C"<<i<<std::endl;
        const Call& c = *constraintVector[i]->cast<Call>();
        
        //It checks what kind of constraint we are dealing with
        
        if (c.id()=="float_lin_eq" || c.id() == "float_lin_le") {
          
          //if it is linear then we can just print out 0
          std::cout<<"n0"<<std::endl;
          os<<"n0"<<std::endl;
        }
        else {
          if (c.id()=="float_times") {
            std::cout<<"o2"<<std::endl;
            os<<"o2"<<std::endl;
          }
          else if (c.id() == "float_div") {
            std::cout<<"o3"<<std::endl;
            os<<"o3"<<std::endl;
          }
          else if (c.id()=="float_pow") {
            std::cout<<"o5"<<std::endl;
            os<<"o5"<<std::endl;
          }
          else if (c.id() == "float_sin") {
            std::cout<<"o41"<<std::endl;
            os<<"o41"<<std::endl;
          }
          else if (c.id() == "float_exp") {
            std::cout<<"o44"<<std::endl;
            os<<"o44"<<std::endl;
          }
          else if (c.id() == "float_cos") {
            std::cout<<"o46"<<std::endl;
            os<<"o46"<<std::endl;
          }
          for (unsigned int j = 0; j < c.args().size() - 1; j++) {
            if (Id* ident = c.args()[j]->dyn_cast<Id>()) {
              std::cout<<"v"<<ident->decl()->payload()<<std::endl;
              os<<"v"<<ident->decl()->payload()<<std::endl;
            }
            else if (FloatLit* fl = c.args()[j]->dyn_cast<FloatLit>()) {
              std::cout<<"n"<<*fl<<std::endl;
              os<<"n"<<*fl<<std::endl;
            }
          }
        }
      }
    }
    
    void printObjectives() {
      for (int i = 0; i < objectiveVector.size(); i++) {
        std::cout<<"O"<<i<<" "<<objTypeVector[i]<<std::endl;
        os<<"O"<<i<<" "<<objTypeVector[i]<<std::endl;
        std::cout<<"n0"<<std::endl;
        os<<"n0"<<std::endl;
      }
    }
    
    void printInitVals() {
      std::cout<<"\n-----------------------------------------------------------\n"<<std::endl;
      std::cout<<"-- printInitVals()"<<std::endl;
      std::cout<<"x"<<initVarVector.size()<<std::endl;
      os<<"x"<<initVarVector.size()<<std::endl;
      for (int i = 0; i < initVarVector.size(); i++) {
        std::cout<<initVarVector[i]<<" "<<*initValVector[i]<<std::endl;
        os<<initVarVector[i]<<" "<<*initValVector[i]<<std::endl;
      }
    }
    
    //The following prints out the r section
    void printRange() {
      std::cout<<"\n-----------------------------------------------------------\n"<<std::endl;
      std::cout<<"-- printRange()"<<std::endl;
      std::cout<<"r"<<std::endl;
      os<<"r"<<std::endl;
      
      //for each of the constraints there will be one range
      for (int i = 0; i < constraintVector.size(); i++) {
        const Call& c = *constraintVector[i]->cast<Call>();
        
        //if our r value is already a float then we can just use its value
        if (FloatLit* fl = c.args()[c.args().size()-1]->dyn_cast<FloatLit>()) {
          if (c.id() == "float_lin_le") {
            
            std::cout<<"1 "<<*fl<<std::endl;
            os<<"1 "<<*fl<<std::endl;
          }
          else {
            std::cout<<"4 "<<*fl<<std::endl;
            os<<"4 "<<*fl<<std::endl;
          }
        }
        
        //It may be our r is actually some variable, in these case we need to move it to the other side and output 0
        else if (Id* ident = c.args()[c.args().size()-1]->dyn_cast<Id>()) {
          if (ident->decl()->payload() < 0) {
            if (VarDecl* vd = variableVector[ident->decl()->payload() - 1]->cast<VarDecl>()) {
              if (TypeInst* ti = vd->ti()->dyn_cast<TypeInst>()) {
                if (Expression* e = ti->domain()) {
                  if (BinOp* bo = e->dyn_cast<BinOp>()) {
                    if (c.id() == "float_lin_le") {
                      std::cout<<"1 "<<*bo->lhs()<<std::endl;
                      os<<"1 "<<*bo->lhs()<<std::endl;
                    }
                    else {
                      std::cout<<"4 "<<*bo->lhs()<<std::endl;
                      os<<"4 "<<*bo->lhs()<<std::endl;
                    }
                  }
                }
              }
            }
          }
          else {
            if (c.id() == "float_lin_le") {
              std::cout<<"1 0"<<std::endl;
              os<<"1 0"<<std::endl;
            }
            else {
              std::cout<<"4 0"<<std::endl;
              os<<"4 0"<<std::endl;
            }
          }
        }
      }
    }
    
    
    //The following prints the bounds for the variables
    void printBounds() {
      
      std::cout<<"\n-----------------------------------------------------------\n"<<std::endl;
      std::cout<<"-- printBounds"<<std::endl;
      
      //This is the section where the bounds are printed out
      std::cout<<"b"<<std::endl;
      os<<"b"<<std::endl;
      for (int i = 0; i < variableVector.size(); i++) {
        
        if (VarDecl* vd = variableVector[i]->cast<VarDecl>()) {
          if (TypeInst* ti = vd->ti()->dyn_cast<TypeInst>()) {
            if (Expression* e = ti->domain()) {
              if (BinOp* bo = e->dyn_cast<BinOp>()) {
                std::cout<<"0 "<<*bo->lhs()<<" "<<*bo->rhs()<<std::endl;
                os<<"0 "<<*bo->lhs()<<" "<<*bo->rhs()<<std::endl;
              }
            }
            else if (relationalOpConstraint[vd->payload()] != -1) {
              const Call& c = *varConstraintVector[relationalOpConstraint[vd->payload()]]->cast<Call>();
              if (c.id() == "float_le") {
                if (Id* ident = c.args()[0]->dyn_cast<Id>()) {
                  std::cout<<"1 "<<*c.args()[1]<<std::endl;
                  os<<"1 "<<*c.args()[1]<<std::endl;
                }
                else {
                  std::cout<<"2 "<<*c.args()[0]<<std::endl;
                  os<<"2 "<<*c.args()[0]<<std::endl;
                }
              }
            }
            else {
              std::cout<<"3"<<std::endl;
              os<<"3"<<std::endl;
            }
          }
        }
      }
    }
    
    //this section prints ouf the k section
    void printkSect() {
      
      std::cout<<"\n-----------------------------------------------------------\n"<<std::endl;
      std::cout<<"-- printkSect()"<<std::endl;
      
      std::cout<<"k"<<variableVector.size()-1<<std::endl;
      os<<"k"<<variableVector.size()-1<<std::endl;
      
      int varTotalSum = 0;
      
      for (int i = 0; i < variableVector.size() - 1; i ++) {
        int varSum = 0;
        for (int j = 0; j < jacobianMatrix.size(); j++) {
          if (jacobianMatrix[j][i] != 0) {
            varSum = varSum + 1;
          }
        }
        
        varTotalSum = varTotalSum + varSum;
        
        std::cout<<varTotalSum<<std::endl;
        os<<varTotalSum<<std::endl;
      }
      
    }
    
    void printJacobian() {
      std::cout<<"\n-----------------------------------------------------------\n"<<std::endl;
      std::cout<<"-- printJacobian()"<<std::endl;
      for (int i = 0; i < constraintVector.size(); i++) {
        std::cout<<"J"<<i<<" "<<jacobianCountVector[i]<<std::endl;
        os<<"J"<<i<<" "<<jacobianCountVector[i]<<std::endl;
        const Call& c = *constraintVector[i]->cast<Call>();
        
//        if (i == 405) {
//          std::cout<<c<<std::endl;
//        }
        
        if (c.id()=="float_lin_eq" || c.id() == "float_lin_le") {
          
          //Assume if it is a float_lin_eq then the first and second arguments will be arrays
          
          //std::cout<<c.args()[1]->dyn_cast<Id>()->decl()->e()->dyn_cast<ArrayLit>()<<std::endl;
          
          ArrayLit* al = NULL;
          if (ArrayLit* al2 = c.args()[1]->dyn_cast<ArrayLit>()) {
            al = al2;
          } else {
            al = c.args()[1]->dyn_cast<Id>()->decl()->e()->cast<ArrayLit>();
          }
          
          ArrayLit* alc = NULL;
          if (ArrayLit* alc2 = c.args()[0]->dyn_cast<ArrayLit>()) {
            alc = alc2;
          } else {
            alc = c.args()[0]->dyn_cast<Id>()->decl()->e()->cast<ArrayLit>();
          }
          
          for (int j = 0; j < al->v().size(); j++) {
            std::cout<<al->v()[j]->dyn_cast<Id>()->decl()->payload()<<" "<<*alc->v()[j]<<std::endl;
            os<<al->v()[j]->dyn_cast<Id>()->decl()->payload()<<" "<<*alc->v()[j]<<std::endl;
          }
          
//          if (ArrayLit* alv = c.args()[1]->dyn_cast<ArrayLit>()) {
//            if (ArrayLit* alc = c.args()[0]->dyn_cast<ArrayLit>()) {
//              for (int j = 0; j < alv->v().size(); j++) {
//                
//                std::cout<<alv->v()[j]->dyn_cast<Id>()->decl()->payload()<<" "<<*alc->v()[j]<<std::endl;
//                os<<alv->v()[j]->dyn_cast<Id>()->decl()->payload()<<" "<<*alc->v()[j]<<std::endl;
//              }
//            }
//            else if (Id* ident = c.args()[0]->dyn_cast<Id>()) {
//              if (ArrayLit* alc = ident->decl()->e()->dyn_cast<ArrayLit>()) {
//                for (int j = 0; j < alv->v().size(); j++) {
//                  std::cout<<alv->v()[j]->dyn_cast<Id>()->decl()->payload()<<" "<<*alc->v()[j]<<std::endl;
//                  os<<alv->v()[j]->dyn_cast<Id>()->decl()->payload()<<" "<<*alc->v()[j]<<std::endl;
//                }
//              }
//            }
//          }
//          else if (c.args()[1]->dyn_cast<Id>()->decl()->payload()<0) {
//            if (Id* ident = knownVariableVector[-(c.args()[1]->dyn_cast<Id>()->decl()->payload())]->dyn_cast<Id>()) {
//              if (ArrayLit* alv = ident->decl()->e()->dyn_cast<ArrayLit>()) {
//                if (ArrayLit* alc = c.args()[0]->dyn_cast<ArrayLit>()) {
//                  for (int j = 0; j < alv->v().size(); j++) {
//                    
//                    std::cout<<alv->v()[j]->dyn_cast<Id>()->decl()->payload()<<" "<<*alc->v()[j]<<std::endl;
//                    os<<alv->v()[j]->dyn_cast<Id>()->decl()->payload()<<" "<<*alc->v()[j]<<std::endl;
//                  }
//                }
//                else if (Id* ident = c.args()[0]->dyn_cast<Id>()) {
//                  if (ArrayLit* alc = ident->decl()->e()->dyn_cast<ArrayLit>()) {
//                    for (int j = 0; j < alv->v().size(); j++) {
//                      std::cout<<alv->v()[j]->dyn_cast<Id>()->decl()->payload()<<" "<<*alc->v()[j]<<std::endl;
//                      os<<alv->v()[j]->dyn_cast<Id>()->decl()->payload()<<" "<<*alc->v()[j]<<std::endl;
//                    }
//                  }
//                }
//              }
//            }
//          }
          
          if (Id* ident = c.args()[2]->dyn_cast<Id>()) {
            if (ident->decl()->payload() >= 0) {
              std::cout<<c.args()[2]->dyn_cast<Id>()->decl()->payload()<<" -1"<<std::endl;
              os<<c.args()[2]->dyn_cast<Id>()->decl()->payload()<<" -1"<<std::endl;
            }
          }
          
        }
        else {
          
          for (int j = 0; j < variableVector.size(); j++) {
            if (jacobianMatrix[i][j] == 1) {
              std::cout<<j<<" 0"<<std::endl;
              os<<j<<" 0"<<std::endl;
            }
            else if (jacobianMatrix[i][j] == -1) {
              std::cout<<j<<" -1"<<std::endl;
              os<<j<<" -1"<<std::endl;
            }
          }
        }
      }
    }
    
    void printGradient() {
      std::cout<<"\n-----------------------------------------------------------\n"<<std::endl;
      std::cout<<"-- printGradient()"<<std::endl;
      for (int i = 0; i < objectiveVector.size(); i++) {
        std::cout<<"G"<<i<<" 1"<<std::endl;
        os<<"G"<<i<<" 1"<<std::endl;
        
        if (Id* ident = objectiveVector[i]->e()->cast<Id>()) {
          std::cout<<ident->decl()->payload()<<" 1"<<std::endl;
          os<<ident->decl()->payload()<<" 1"<<std::endl;
        }
      }
    }
    
  };

  
  void NLPrinter::print(const Model* m) {
    
    //This will replicate the above code for the plain printer
    //to aavoid confusion we will create a new class called nlPlainPrinter
    
    nlPlainPrinter p(_os);
    for (unsigned int i = 0; i < m->size(); i++) {
      std::cout<<i<<std::endl;
//      std::cout<<"*m = "<<m<<std::endl;
      p.nlp((*m)[i]);
    }
    p.updateJacobianMatrix_aux();
    p.printVariables(); //This is where we call the function method to print the variables
    //            p.unknownVariables();
    p.printHeader();
    p.printConstraints();   //This is where we print the constraints
    p.printObjectives();
    p.printInitVals();
    p.printRange(); //This is where we print the range
    p.printBounds(); //This is where we print the bounds
    p.printkSect(); //This is where we print the k section
    p.printJacobian();  //This is where we print the Jaconian section
    p.printGradient();
  }
}

