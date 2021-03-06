#ifndef VARIABLEIDMAPPING_H
#define VARIABLEIDMAPPING_H

/*************************************************************
 * Copyright: (C) 2012 by Markus Schordan                    *
 * Author   : Markus Schordan                                *
 * License  : see file LICENSE in the CodeThorn distribution *
 *************************************************************/

#include <string>
#include <map>
#include <vector>
#include <boost/unordered_set.hpp>

#include "RoseAst.h"
#include "SgNodeHelper.h"

using namespace std;

namespace CodeThorn {

  class VariableId;
  typedef string VariableName;
/*! 
  * \author Markus Schordan
  * \date 2012.
 */
class VariableIdMapping {
  /* TODO: possible workaround: because the AST implementation is not completed for the following cases:
     1) SgInitializedName in forward declaration (symbol=0)
     2) CtorInitializerList (symbol=0)
     the symbol is missing in both cases, a VariableId can be assign to the passed SgInitializedName pointer.
  */

 public:
  //typedef boost::unordered_set<VariableId> VariableIdSet;
  typedef set<VariableId> VariableIdSet;

  // the computation of the CodeThorn-defined ROSE-based variable-symbol mapping
  // creates a mapping of variableNames and its computed UniqueVariableSymbol
  void computeVariableSymbolMapping(SgProject* project);

  /* create a new unique variable symbol (should be used together with deleteUniqueVariableSymbol)
     this is useful if additional (e.g. temporary) variables are introduced in an analysis
     this function does NOT insert this new symbol in any symbol table
   */
  VariableId createUniqueTemporaryVariableId(string name);

  // delete a unique variable symbol (should be used together with createUniqueVariableSymbol)
  void deleteUniqueTemporaryVariableId(VariableId uniqueVarSym);

  class UniqueTemporaryVariableSymbol : public SgVariableSymbol {
  public:
    // Constructor: we only allow this single constructor
    UniqueTemporaryVariableSymbol(string name);
    // Destructor: default is sufficient
    
    // overrides inherited get_name (we do not use a declaration)
    SgName get_name() const;
  private:
    string _tmpName;
  };

  typedef size_t VarId;
  VariableId variableId(SgVariableDeclaration* decl);
  VariableId variableId(SgVarRefExp* varRefExp);
  VariableId variableId(SgInitializedName* initName);
  VariableId variableId(SgSymbol* sym);
  VariableId variableIdFromCode(int);
  SgSymbol* getSymbol(VariableId varId);
  bool isTemporaryVariableId(VariableId varId);
  string variableName(VariableId varId);
  string uniqueLongVariableName(VariableId varId);
  string uniqueShortVariableName(VariableId varId);

  void registerNewSymbol(SgSymbol* sym);
  void toStream(ostream& os);
  void generateDot(string filename,SgNode* astRoot);

  VariableIdSet getVariableIdSet();

  VariableIdSet determineVariableIdsOfVariableDeclarations(set<SgVariableDeclaration*> varDecls);
  VariableIdSet determineVariableIdsOfSgInitializedNames(SgInitializedNamePtrList& namePtrList);

  //private: (soon!)
  // checks whether the computed CodeThorn-defined ROSE-based variable-symbol mapping is bijective.
  bool isUniqueVariableSymbolMapping();
  void reportUniqueVariableSymbolMappingViolations();
 private:

  void generateStmtSymbolDotEdge(std::ofstream&, SgNode* node,VariableId id);
  string generateDotSgSymbol(SgSymbol* sym);
  typedef pair<string,SgSymbol*> MapPair;
  set<MapPair> checkSet;
  typedef pair<VariableId,VariableName> PairOfVarIdAndVarName;
  typedef set<PairOfVarIdAndVarName> TemporaryVariableIdMapping;
  TemporaryVariableIdMapping temporaryVariableIdMapping;
  VariableId addNewSymbol(SgSymbol* sym);
  // used for mapping in both directions
  vector<SgSymbol*> mappingVarIdToSym;
  map<SgSymbol*,size_t> mappingSymToVarId;
}; // end of class VariableIdMapping

 typedef VariableIdMapping::VariableIdSet VariableIdSet;

/*! 
  * \author Markus Schordan
  * \date 2012.
 */
class VariableId {
  friend class VariableIdMapping;
  friend bool operator<(VariableId id1, VariableId id2);
  friend bool operator==(VariableId id1, VariableId id2);
  friend class ConstraintSetHashFun; // TODO: investigate why getSymbol needs to be public
  //friend size_t hash_value(const CodeThorn::VariableId&);
 public:
  VariableId();
  string toString() const;
  int getIdCode() const { return _id; }
  // we intentionally do not provide a constructor for int because this would clash 
  // with overloaded functions that are using ConstIntLattice (which has an implicit 
  // type conversion for int)
  void setIdCode(int id) {_id=id;}
  //string variableName() const;
  //string longVariableName() const;
  //VariableId(SgSymbol* sym);
  bool isValid() { return _id!=-1; }
 public:
  //SgSymbol* getSymbol() const; // only public because of ContraintSetHashFun
 private: 
  //SgSymbol* sym;
  int _id;
};

 size_t hash_value(const CodeThorn::VariableId& vid);

bool operator<(VariableId id1, VariableId id2);
bool operator==(VariableId id1, VariableId id2);
bool operator!=(VariableId id1, VariableId id2);

} // end of namespace CodeThorn

#endif
