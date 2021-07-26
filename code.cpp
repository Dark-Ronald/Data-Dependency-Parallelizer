#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <tuple>

using namespace std;

char keyChars[] = {'~', '!', '#', '%', '^', '&', '*', '(', ')', '-', '+', '=', '{', '}', '|', ',', '.',
  '<', '>', '\'', '"', ';'};
const char* twoCharKeys[] = {"+=", "++", "-=", "--", "==", "/=", "*="};
//string loopKeys = {"while", "for", "do"};
vector<const char*> loopKeys;
vector<const char*> wordKeys;

class expression;
class statement;
class scope;
class scopeTree;
class graphNode;
class function;
class functionTracker;
class nodeContainer;

bool isKeyChar(char c);
bool isLoop(statement*& state);
bool isBracket(statement*& state);
bool isWhiteSpace(char c);
bool isPrecomp(statement*& c);
bool is2CharKey(statement*& state1, statement*& state2);
bool isOperator(statement*& state);
bool areEqual(vector<char>& s1, vector<char>& s2);
bool areEqual(vector<char>& s1, char* s2);
bool areEqual(vector<char>& s1, string s2);
bool areEqual(vector<char>& s1, char c);
bool isKeyWord(statement*& state);
string toString(vector<char>& s);
bool isBranch(statement*& state);
//void setType(statement*& state);
bool isClassDecl(statement*& state, statement*& state2);
//bool isFunc(statement*& state1, statement*& state2);
bool isFunc(vector<statement*>& statements, int n, scopeTree* scopes);
void createFuncsFromGraphs(nodeContainer* container, bool insideFuncDef, vector<statement*>& finalExpressions, functionTracker* funcTrack, int ng, scopeTree* scopes, int retType, function* callingFunc);
vector<statement*> createAndInjectFuncs(nodeContainer* container, scopeTree* scopes);
void printExpressions(vector<statement*>& expressions, int* i);
void printExpressions(vector<statement*>& expressions);

class statement {
public:
  enum {undefKeyChar = -2, undefName, typeDecl, funcCall, datum, loop, op, bracket, precomp, keyWord, expr, semiCol, branch, typeDef, funcRet};
  //enum datum {lit, var, number, string};
  //enum typeDecl {objectDecl, classDecl, structDecl};
  //enum loop {forLoop, whileLoop, doLoop};
  //enum bracket {round, brace, square};
  //enum keyW {ret, include, define, use, nspace, newK, del};
  //enum op {add, sub, mult, div, assign, greater, less, amp, point, deref};
  vector<char> codeString;
  scope* layer;
  vector<statement*> statements;
  int nStatements = 0; //number of statements statement accepts (i.e. a while loop accepts 2, a condition and a body)
  vector<int> types; //types[0] will be base type, specific types are stored after
  

  statement() {
    types.push_back(-3);
  }
};

class datum : public statement {
public:
  enum {lit, var, number, string};
};

class typeDecl : public statement {
public:
  enum {objectDecl, classDecl, structDecl, funcDecl};
};

class typeDef : public statement {
public:
  enum {classDef, structDef, funcDef};
};

class loop : public statement {
public:
  enum {forLoop, whileLoop, doLoop};
};

class bracket : public statement {
public:
  enum {round, brace, square, opening, closing};
};

class keyW : public statement {
public:
  enum {ret, include, define, use, nspace, newK, del};
};

class op : public statement {
public:
  enum {add, sub, mult, div, assign, greater, less, amp, point, deref};
};

class branch : public statement {
public:
  enum {ifBranch, elseIf, elseBranch};
};

/*
class statement {
public:
  scope* layer;
  enum {undefKeyChar = -2, undefName, typeDecl, func, datum, loop, op, bracket, precomp, keyWord, expr, semiCol};
  int type = -3; //undefined keyChar = -2, undefined name = -1, typeDecl = 0, funcCall = 1, datum = 2, loop = 3, operator = 4, bracket = 5
  vector<char> name;

};

class datum : public statement {
public:
  enum {lit, var, number, string};
  int dType;
  int daType;
};

class typeDecl : public statement {
public:
  enum {objectDecl, classDecl, structDecl};
  int declType;
  vector<statement> decl;
};

class objectDecl : public typeDecl {
public:
};

class classDecl : public typeDecl {
public:

};

class structDecl : public typeDecl {
public:

};

class func : public statement {
public:
  expression* args;
  int retType;
  expression* body;
};

class loop : public statement {
public:
  enum {forLoop, whileLoop, doLoop};
  int lType; //0 is for, 1 is while, 2 is do while
  expression* body;
};

class forLoop : public loop {
public:
  expression* inti;
  expression* cond;
  expression* incr;
};

class whileLoop : public loop {
public:
  expression* cond;
};

class doLoop : public loop {
public:
  expression* cond;
};

class bracket : public statement {
public:
  enum {round, brace, square};
  bool opening;  //true for opening, false for closing
  int bType; //0 for round, 1 for brace, 2 for square
};

class openingBracket : public bracket {
public:
  expression* body;
};

class keyW : public statement {
public:
  enum {ret, include, define, use, nspace, newK, del};
  //statement* param;
  int kType;
};

class oneParamKeyWord : public keyW {
public:
  statement* param;
};

class definition : public keyW {
public:

};

class precomp : public statement {
public:
  keyW* directive;
};

class op : public statement {
public:
  enum {add, sub, mult, div, assign, greater, less, amp, point, deref};
  int oType;
};

class compOp : public statement {
public:
  int compType;
};

class genStatement : public statement {
public:
  vector<genStatement*> statements;
  vector<int> types;
};

class expression : public statement {
public:
  vector<statement> statements;

  expression() {
    type = statement::expr;
  }
};
*/

class document {
public:
	vector<vector<char> > lines;

	document(string fileName) {
		ifstream fileRead(fileName);
		string line;
		char c;
		while (!fileRead.get(c).eof()) {
			cout << (int)c << "c: (" << c << ")" << endl;
			line += c;
			if (c == '\n') {
				vector<char> temp(line.c_str(), line.c_str() + line.size() + 1);
				this->addLine(temp);
				line = "";
			}
			fileRead.peek();
			if (fileRead.eof()) {
				vector<char> temp(line.c_str(), line.c_str() + line.size() + 1);
				this->addLine(temp);
			}
		}
		fileRead.close();
	}

	void addLine(vector<char>& line) {
		lines.push_back(line);
	}

	void printDocument() {
		for (int i = 0; i < lines.size(); i++) {
			cout << "line " << i + 1 << ":  \t";
			for (int j = 0; j < lines[i].size(); j++) {
				cout << lines[i][j];
			}
		}
	}

	static void writeDocument(string fileName, vector<statement*>& expressions) {
		string docName(fileName, 0, fileName.length() - 4);
		ofstream fileWrite(docName + "_parallel.cpp");

		for (statement* expr : expressions) {
			for (statement* state : expr->statements) {
				for (char c : state->codeString) {
					fileWrite.put(c);
				}
				fileWrite.put(' ');
			}
			fileWrite.put('\n');
		}
		fileWrite.close();
	}
};

class scope {
public:
  scope* parent;
  vector<scope*> children;
  vector<statement*> statements;
  vector<int> scopeDTypes;

  void add(statement* state) {
    statements.push_back(state);
  }
};

class scopeTree {
public:
  scope* root;
  vector<string> dTypes;

  scopeTree() {
    dTypes.push_back("123");
    dTypes.push_back("abc");
  }

  scope* newScope(scope* s) {
    scope* temp = new scope();
    s->children.push_back(temp);
    temp->parent = s;
    return temp;
  }

  scope* init() {
    root = new scope();
    root->parent = NULL;
    return root;
  }

  scope* parentScope(scope* s) {
    return s->parent;
  }

  int setType(statement*& state) {
    int ret;
    bool found = false;
    for (int i = 0; i < dTypes.size(); i++) {
      if (areEqual(state->codeString, dTypes[i])) {
        found = true;
        ret = i;
        state->layer->scopeDTypes.push_back(i);
      }
    }
    if (!found) {
      dTypes.push_back(toString(state->codeString));
      ret = dTypes.size() - 1;
      state->layer->scopeDTypes.push_back(ret);
    }
    return ret;
  }

  void addType(statement*& state) {
    dTypes.push_back(toString(state->codeString));
    state->layer->scopeDTypes.push_back(dTypes.size() - 1);
  }

  //checks if a variable with state->codeString is in the scope of state
  bool hasVar(statement*& state) {
    bool ret = false;
    for (scope* curScope = state->layer; curScope != NULL; curScope = parentScope(curScope)) {
      for (statement*& s : curScope->statements) {
        if ((s->types[0] == statement::datum) && (areEqual(state->codeString, s->codeString))) {
          if (state->types[0] == statement::undefName) {
            statement* temp = new statement();
            temp->codeString = state->codeString;
            temp->types[0] = statement::datum;
            temp->layer = state->layer;
            temp->types.push_back(s->types[1]);
            temp->types.push_back(datum::var);  //change this to var?
            delete state;
            state = temp;
          }
          return true;
        }
        // if ((s->types[0] == statement::typeDecl) && (areEqual(state->codeString, s->codeString))) {
        //   if (state->types[0] == statement::undefName) {
        //     typeDecl* sT = (typeDecl*)s;
        //     if (sT->declType == typeDecl::objectDecl) {
        //       objectDecl* temp = new objectDecl();
        //       temp->codeString = state->codeString;
        //       temp->types[0] = statement::typeDecl;
        //       temp->layer = state->layer;
        //       temp->declType = typeDecl::objectDecl;
        //       delete state;
        //       state = temp;
        //     }
        //     else if (sT->declType == typeDecl::classDecl) {
        //       classDecl* temp = new classDecl();
        //       temp->codeString = state->codeString;
        //       temp->types[0] = statement::typeDecl;
        //       temp->layer = state->layer;
        //       temp->declType = typeDecl::classDecl;
        //       delete state;
        //       state = temp;
        //     }
        //     else if (sT->declType == typeDecl::structDecl) {
        //       structDecl* temp = new structDecl();
        //       temp->codeString = state->codeString;
        //       temp->types[0] = statement::typeDecl;
        //       temp->layer = state->layer;
        //       temp->declType = typeDecl::structDecl;
        //       delete state;
        //       state = temp;
        //     }
        //   }
        //   ret = true;
        // }
      }
    }
    return ret;
  }

  statement* getPrevOccurence(statement* state) {
    for (scope* curScope = state->layer; curScope != NULL; curScope = parentScope(curScope)) {
      if (curScope == state->layer) {
        for (int i = 0; state != curScope->statements[i]; i++) { //if current state is layers state then start from beginning so that occurences after current statement are not used
          if ((curScope->statements[i]->types[0] == statement::datum) && (areEqual(state->codeString, curScope->statements[i]->codeString))) {
            return curScope->statements[i];
          }
        }
      }
      else {
        for (int i = curScope->statements.size() - 1; i >= 0; i--) {
          if ((curScope->statements[i]->types[0] == statement::datum) && (areEqual(state->codeString, curScope->statements[i]->codeString))) {
            return curScope->statements[i];
          }
        }
      }
    }
    return state;
  }
};

/*
class scope {
public:
  vector<int> layer;
  vector<statement*> statements;
  vector<int> scopeDTypes;

  void add(statement* state) {
    statements.push_back(state);
  }

};

class scopeTree {
public:
  vector<int> layers;
  vector<vector<scope*> > scopes;
  vector<string> dTypes;

  scopeTree() {
    dTypes.push_back("123");
    dTypes.push_back("abc");
  }

  scope* newScope(scope* s) {
    cout << "creating new scope" << endl;
    scope* temp = new scope();
    if (s->layer.size() == layers.size()) {
      cout << "adding new layer" << endl;
      cout << "scopes size: " << scopes.size() << endl;
      layers.push_back(0);
      temp->layer = layers;
      vector<scope*> temp2;
      temp2.push_back(temp);
      scopes.push_back(temp2);
    }
    else {
      cout << "expanding existing layer" << endl;
      layers[s->layer.size()]++; //not layers[s->layer.size() - 1] because the new scope is a child
      scopes[s->layer.size()].push_back(temp);
      for (int i = 0; i < s->layer.size() + 1; i++) {
        temp->layer.push_back(layers[i]);
      }
    }
    cout << "scopeTree layers: " << endl;
    for (int i = 0; i < scopes.size(); i++) {
      for (int j = 0; j < scopes[i].size(); j++) {
        cout << scopes[i][j] << ", ";
      }
      cout << endl;
    }
    return temp;
  }

  scope* init() {
    layers.push_back(0);
    scope* temp = new scope();
    temp->layer = layers;
    vector<scope*> temp2;
    temp2.push_back(temp);
    scopes.push_back(temp2);
    return temp;
  }

  scope* parentScope(scope* s) {
    if (s->layer.size() == 1) {
      return NULL;
    }
    cout << "not at global scope" << endl;
    cout << "parent scope is: scopes[" << s->layer.size() - 2 << "][" << s->layer[s->layer.size() - 2] << "]" << endl;
    return scopes[s->layer.size() - 2][s->layer[s->layer.size() - 2]];
  }

  int setType(statement*& state) {
    int ret;
    bool found = false;
    for (int i = 0; i < dTypes.size(); i++) {
      if (areEqual(state->codeString, dTypes[i])) {
        found = true;
        ret = i;
        state->layer->scopeDTypes.push_back(i);
      }
    }
    if (!found) {
      dTypes.push_back(toString(state->codeString));
      ret = dTypes.size() - 1;
      state->layer->scopeDTypes.push_back(ret);
    }
    return ret;
  }

  void addType(statement*& state) {
    dTypes.push_back(toString(state->codeString));
    state->layer->scopeDTypes.push_back(dTypes.size() - 1);
  }

  static void printScope(scope* s) {
    for (int i = 0; i < s->layer.size(); i++) {
      cout << s->layer[i] << ", ";
    }
    cout << endl;
  }

  //checks if a variable with state->codeString is in the scope of state
  bool hasVar(statement*& state) {
    bool ret = false;
    for (scope* curScope = state->layer; curScope != NULL; curScope = parentScope(curScope)) {
      for (statement*& s : curScope->statements) {
        if ((s->types[0] == statement::datum) && (areEqual(state->codeString, s->codeString))) {
          if (state->types[0] == statement::undefName) {
            statement* temp = new statement();
            temp->codeString = state->codeString;
            temp->types[0] = statement::datum;
            temp->layer = state->layer;
            temp->types.push_back(s->types[1]);
            temp->types.push_back(datum::lit);
            delete state;
            state = temp;
          }
          return true;
        }
        // if ((s->types[0] == statement::typeDecl) && (areEqual(state->codeString, s->codeString))) {
        //   if (state->types[0] == statement::undefName) {
        //     typeDecl* sT = (typeDecl*)s;
        //     if (sT->declType == typeDecl::objectDecl) {
        //       objectDecl* temp = new objectDecl();
        //       temp->codeString = state->codeString;
        //       temp->types[0] = statement::typeDecl;
        //       temp->layer = state->layer;
        //       temp->declType = typeDecl::objectDecl;
        //       delete state;
        //       state = temp;
        //     }
        //     else if (sT->declType == typeDecl::classDecl) {
        //       classDecl* temp = new classDecl();
        //       temp->codeString = state->codeString;
        //       temp->types[0] = statement::typeDecl;
        //       temp->layer = state->layer;
        //       temp->declType = typeDecl::classDecl;
        //       delete state;
        //       state = temp;
        //     }
        //     else if (sT->declType == typeDecl::structDecl) {
        //       structDecl* temp = new structDecl();
        //       temp->codeString = state->codeString;
        //       temp->types[0] = statement::typeDecl;
        //       temp->layer = state->layer;
        //       temp->declType = typeDecl::structDecl;
        //       delete state;
        //       state = temp;
        //     }
        //   }
        //   ret = true;
        // }
      }
    }
    return ret;
  }

  // bool hasType(statement*& state) {
  //   for (int i = 0; i < dTypes.size(); i++) {
  //     if (areEqual(state->codeString, dTypes[i])) {
  //       state->layer->scopeDTypes.push_back(i);
  //     }
  //   }
  // }
};
*/

class nodeContainer {
public:
  vector<graphNode*> nodes;
  graphNode* owner;
  vector<vector<graphNode*> > nodeGraphs;
  vector<graphNode*> finalNodes;
};


class graphNode {
public:
  //vector<vector<char> > lines;
  //data* dataDependencies;
  //vector<statement*> expressions;
  statement* expression;
  vector<graphNode*> parents;
  vector<graphNode*> children;
  nodeContainer* container = NULL; //used to know where code in node needs to be placed inside of when writing back to file
  bool traveled = false;
  nodeContainer* owningContainer = NULL;

  void findAllConnectionsInContainer(vector<graphNode*>& nodeGraph) {
    if (!traveled) {
      traveled = true;
      nodeGraph.push_back(this);
      for (graphNode*& parentNode : parents) {
        if ((parentNode->owningContainer == owningContainer) && !(parentNode->traveled)) {
          parentNode->findAllConnectionsInContainer(nodeGraph);
        }
      }
      for (graphNode*& childNode : children) {
        if ((childNode->owningContainer == owningContainer) && !(childNode->traveled)) {
          childNode->findAllConnectionsInContainer(nodeGraph);
        }
      }
    }
  }

  // void hasScope(scope* scope) {
  //   for (scope* s : exprScopes) {
  //     if
  //   }
  // }
};

class dependGraph {
public:

  struct existD {
    statement* datum;
    graphNode* node;
  };

  vector<graphNode*> rootNodes;
  scopeTree* scopes;
  //vector<tuple<statement*, graphNode*> > existingDatum;
  vector<struct existD> existingDatum;

  dependGraph() {
  }

  nodeContainer* buildDependGraph(document& file, scopeTree*& st) {
    //vector<expression> expressions;
    vector<statement*> firstRunCodeParse;
    bool isString = false;
    int stringType = 0;
    bool number = false;
    bool precomp = false;
    statement* curStatement = new statement();
    //expression curExpr;
    for (vector<char>& line : file.lines) {
      cout << "buildDependGraph | file line: " << toString(line) << endl;
      //TODO: add handler for comments
      for (int i = 0; i < line.size(); i++) {
        char c = line[i];
        cout << "new char" << endl;
        if (precomp) {
          if (c == '\n') {
            precomp = false;
            firstRunCodeParse.push_back(curStatement);
            curStatement = new statement();
          }
          else {
            curStatement->codeString.push_back(c);
          }
        }
        else if ((c == '"') || ((int)c == 39)) {
          if (isString && (stringType == (int)c)) {
            curStatement->codeString.push_back(c);
            statement* temp = new statement();
            temp->codeString = curStatement->codeString;
            temp->types[0] = statement::datum;
            temp->types.push_back(1);
            temp->types.push_back(datum::lit);
            delete curStatement;
            curStatement = temp;
            isString = false;
            stringType = 0;
          }
          else {
            firstRunCodeParse.push_back(curStatement);
            curStatement = new statement();
            curStatement->codeString.push_back(c);
            isString = true;
            stringType = (int)c;
          }
        }
        else if (isString) {
          curStatement->codeString.push_back(c);
        }
        //assumes number literals can only come after a keyChar
        else if (isNumber(c) && (((curStatement->types[0] == statement::datum) && (curStatement->types[2] == datum::lit)) || (curStatement->codeString.empty()))) {
          cout << "is number" << endl;
          if ((curStatement->types[0] == statement::datum) && (curStatement->types[2] == datum::lit)) {
            cout << "adding to current number" << endl;
            curStatement->codeString.push_back(c);
          }
          else {
            cout << "creating new number" << endl;
            statement* temp = new statement();
            temp->codeString = curStatement->codeString;
            temp->types[0] = statement::datum;
            temp->types.push_back(0);
            temp->types.push_back(datum::lit);
            delete curStatement;
            curStatement = temp;
            curStatement->codeString.push_back(c);
            cout << "number type: " << curStatement->types[0] << endl;
          }

        }
        else if (c == '#') {
          precomp = true;
          curStatement->codeString.push_back(c);
          curStatement->types[0] = statement::precomp;
          cout << "buildDependGraph | pushing back: " << toString(curStatement->codeString) << endl;
          //firstRunCodeParse.push_back(curStatement);
          //curStatement = new statement();
          //break;
        }
        else if (isWhiteSpace(c)) {
          cout << "isWhiteSpace" << endl;
          if (curStatement->codeString.size() > 0) {
            cout << "abc" << endl;
            cout << "buildDependGraph | pushing back: " << toString(curStatement->codeString) << endl;
            firstRunCodeParse.push_back(curStatement);
            curStatement = new statement();
          }
        }
        else if (isKeyChar(c)) {

          if (curStatement->codeString.empty()) {
            curStatement->codeString.push_back(c);
            if ((int)c == 59) {
              curStatement->types[0] = statement::semiCol;
            }
            else {
              curStatement->types[0] = statement::undefKeyChar;
            }
            cout << "buildDependGraph | pushing back from empty: " << toString(curStatement->codeString) << endl;
            firstRunCodeParse.push_back(curStatement);
            curStatement = new statement();
          }
          else if (curStatement->types[0] != statement::undefKeyChar) { //if curStatement is not undefined keychar
            //curExpr.push_back(curStatement);
            cout << "buildDependGraph | pushing back: " << toString(curStatement->codeString) << " | type: " << curStatement->types[0] << endl;
            firstRunCodeParse.push_back(curStatement);
            curStatement = new statement();
            if ((int)c == 59) {
              curStatement->types[0] = statement::semiCol;
            }
            else {
              curStatement->types[0] = statement::undefKeyChar;
            }

            curStatement->codeString.push_back(c);
            cout << "buildDependGraph | pushing back: " << toString(curStatement->codeString) << endl;
            firstRunCodeParse.push_back(curStatement);
            curStatement = new statement();
          }

        }
        else {
          cout << "adding to name: " << c << ": " << (int)c << endl;
          curStatement->codeString.push_back(c);
        }
        if (curStatement->types[0] == -3) {
          curStatement->types[0] = statement::undefName;
        }

      }
      //finalizeExpression(curExpr);
    }
    //firstRunCodeParse.push_back(curStatement); //leaving this commented assumes that all files will end with either ';' or '}'
    cout << "buildDependGraph | firstRunCodeParse" << endl;
    //finalizeExpressions(firstRunCodeParse);
    dependGraph::firstRunCodeParse(firstRunCodeParse);

    for (int i = 0; i < firstRunCodeParse.size(); i++) {
      cout << toString(firstRunCodeParse[i]->codeString) << " | " << firstRunCodeParse[i]->types[0] << endl;
      //scopeTree::printScope(firstRunCodeParse[i]->layer);
    }

    vector<statement*> parsedCode = secondRunCodeParse(firstRunCodeParse);
    cout << "have parsed code" << endl;
    int* i = new int();
    *i = 0;
    printExpressions(parsedCode, i);

    //vector<statement*>& parsedCode = firstRunCodeParse;

    nodeContainer* container = genNodes(parsedCode);

    markGraphsInContainers(container);
    st = scopes;
    return container;

    /*
    for (expression*& expr : parsedCode) {
      graphNode* node = new graphNode;
      node->expressions.push_back(expr);
      for (genStatement*& state : expr) { //if every datum is independent then node is a root node
        // if ((state->types[0] == statement::datum) && !(scopes->isFirstOccurence(state))) {
        //   node->parents.push_back(getPreviousOccurence(state));
        //   node->parents[node->parents.size() - 1]->children.push_back(node); //add node as child of parent
        // }
        for (statement* states : state->statements) {
          if ((states->types[0]s[0] == statement::datum) && !(scopes->isFirstOccurence)) {
            setOwnerDependencies(getPreviousOccurence(state));
          }
        }
      }
    }
    */
  }



  nodeContainer* genNodes(vector<statement*>& parsedCode) {
    cout << "genNodes" << endl;
    statement* pC = new statement();
    pC->types[0] = statement::expr;
    pC->statements = parsedCode;
    graphNode* temp = new graphNode();
    nodeContainer* cont = new nodeContainer();
    cont->nodes.push_back(temp);
    // for (statement*& state : parsedCode) {
    //   genNodes(state, cont);
    // }
    genNodes(pC, cont);
    return temp->container;
  }

  bool genNodes(statement* expressions, nodeContainer* container) {
    graphNode* node = NULL;
    bool hasSubExpr = false;
    bool exprOfExprs = true;
	bool ifState = false;
	bool dependIf = false;
	bool elseState = false;
    //cout << "genNodes | setting container reference" << endl;
    //nodeContainer*& cont = container->nodes[container->nodes.size() - 1]->container;
    //cout << "genNodes | checking statements in expression for exprOfExprs" << endl;
    for (statement*& state : expressions->statements) {
      if (state->types[0] != statement::expr) {
        exprOfExprs = false;
      }
    }
    if (exprOfExprs) {
      cout << "exprOfExprs" << endl;
      cout << "creating container" << endl;
      container->nodes[container->nodes.size() - 1]->container = new nodeContainer();
      cout << "setting new container owner" << endl;
      container->nodes[container->nodes.size() - 1]->container->owner = container->nodes[container->nodes.size() - 1];
    }
    else {
      cout << "not exprOfExprs" << endl;
      node = new graphNode();
	  node->expression = new statement();
      container->nodes.push_back(node);
	  node->owningContainer = container;
    }
    cout << "genNodes | at loop" << endl;
    cout << "printing expressions =========================================================" << endl;
    int* x = new int();
    *x = 0;
    printExpressions(expressions->statements, x);
    delete x;
    cout << endl;
    cout << "done printing expressions ============================================" << endl;
    for (statement*& state : expressions->statements) {
      if (state->types[0] == statement::expr) {
		  if (dependIf) {
			  ifState = false;
			  if ((state->statements[0]->types[0] == statement::branch) && (state->statements[0]->types[1] == branch::elseBranch)) {
				  elseState = true;
			  }
		  }
        if (exprOfExprs) {
          cout << "recursive call as exprOfExprs" << endl;
          dependIf = genNodes(state, container->nodes[container->nodes.size() - 1]->container);
          cout << "finished recursive call of exprOfExprs" << endl;
        }
        else {
          cout << "recursive call" << endl;
          dependIf = genNodes(state, container);
          cout << "finished recursive call" << endl;
        }
        hasSubExpr = true;
		if (elseState) {
			elseState = false;
			container->nodes[container->nodes.size() - 1]->container->nodes[container->nodes[container->nodes.size() - 1]->container->nodes.size() - 1]->parents.push_back(container->nodes[container->nodes.size() - 1]->container->nodes[container->nodes[container->nodes.size() - 1]->container->nodes.size() - 2]);
			container->nodes[container->nodes.size() - 1]->container->nodes[container->nodes[container->nodes.size() - 1]->container->nodes.size() - 2]->children.push_back(container->nodes[container->nodes.size() - 1]->container->nodes[container->nodes[container->nodes.size() - 1]->container->nodes.size() - 1]);
		}
      }
      else {
		  if ((state->types[0] == statement::branch) && (state->types[1] == branch::ifBranch)) {
			  ifState = true;
		  }
        else if ((state->types[0] == statement::datum) && (state->types[2] != datum::lit)) {
          statement* temp = scopes->getPrevOccurence(state);
          if (temp != state) {
            cout << "expression ============================" << endl;
            printExpressions(expressions->statements);
            cout << "is dependent on" << endl;
            node->parents.push_back(getDatum(temp));
			node->parents[node->parents.size() - 1]->children.push_back(node);
            setContainerDependencies(node, node->parents[node->parents.size() - 1]);
            printExpressions(node->parents[node->parents.size() - 1]->expression->statements);
          }
          else {
            struct existD t = {temp, node};
            existingDatum.push_back(t);
          }
        }
		node->expression->statements.push_back(state);
      }
    }
    //if (!hasSubExpr) {
      //node->expressions.push_back(expressions);
	  //if (node->expression !)
      //node->expression = expressions;
    //}
	return ifState;
  }

  graphNode* getDatum(statement* state) {
    for (int i = 0; i < existingDatum.size(); i++) {
      if (existingDatum[i].datum == state) {
        return existingDatum[i].node;
      }
    }
    return NULL; //function should never get here, since it is only called when state exists in existingDatum
  }

  void setContainerDependencies(graphNode* node, graphNode* parent) {
    if (node->owningContainer != parent->owningContainer) {
      nodeContainer* cont;
      for (cont = node->owningContainer; cont->owner->owningContainer != parent->owningContainer; cont = cont->owner->owningContainer) {}
      bool hasDepend = false;
      for (graphNode* n : cont->owner->parents) {
        if (n == cont->owner) {
          hasDepend = true;
        }
      }
      if (!hasDepend) {
        cont->owner->parents.push_back(parent);
        cont->owner->parents[cont->owner->parents.size() - 1]->children.push_back(cont->owner);
      }
    }
  }

  void markGraphsInContainers(nodeContainer* container) {
    cout << "markGraphsInContainers" << endl;
    for (graphNode*& node : container->nodes) {
      vector<graphNode*> nodeGraph;
      if (!(node->traveled)) {
        node->findAllConnectionsInContainer(nodeGraph);
        container->nodeGraphs.push_back(nodeGraph);
      }
      if (node->container != NULL) {
        markGraphsInContainers(node->container);
      }
    }
  }

  /*
  void genChildNodes(genStatement* state) {
    for (genStatement*& genState : state->statements) {
      if (genState)
      graphNode* node = new graphNode();
      node->expressions.push_back(expr);
    }
  }

  int main() {
    int a = 5;
    if (a == 3) {
      int b = 2;
    }
  }

  func: (vector<statement> args), (vector<statements> statements): expression: typeDecl, datum, op, datum
  */

  /*
  //Line by line expression finalizer
  expression* finalizeExpression(expression* expr) {
    for (int i = 0; i < expr->statements.size(); i++) {
      if (expr->statements[i]->codeString == -1) {
        if (isLoop(expr->statements[i])) {
          i++; //next statement is gaurnteed to be either
          if (((*loop)expr->statements[i])->lType == 2) {
            expr->statements[i]
        }
      }
    }
  }
  */

  void firstRunCodeParse(vector<statement*>& fRCP) {
    scopes = new scopeTree();
    scope* curScope = scopes->init();
    cout << "initial scope: ";
    //scopeTree::printScope(curScope);
    //statement* curStatement = new expression();
    int bracketLevel = 0;
    int exprLoc = 0;
    for (int i = 0; i < fRCP.size(); i++) {
      fRCP[i]->layer = curScope;
      
      cout << "finalizeExpressions | fRCP[" << i << "] name: " << toString(fRCP[i]->codeString) << endl;
      if (fRCP[i]->types[0] == statement::semiCol) {
        exprLoc = 0;
      }
      else {
        exprLoc++;
      }
      if (fRCP[i]->types[0] == statement::undefName) {
        cout << "is undefName" << endl;
        if (isLoop(fRCP[i])) {
        }
        else if (isBranch(fRCP[i])) {

        }
        else if (isKeyWord(fRCP[i])) {

        }
        else if ((i + 2 < fRCP.size()) && (isClassDecl(fRCP[i], fRCP[i + 2]))) {
          cout << "is class decl" << endl;
          scopes->addType(fRCP[i + 1]);
          statement* temp = new statement();
          temp->codeString = fRCP[i + 1]->codeString;
          temp->types[0] = statement::typeDecl;
          temp->layer = fRCP[i + 1]->layer;
          temp->types.push_back(typeDecl::objectDecl);
          delete fRCP[i + 1];
          fRCP[i + 1] = temp;
        }
        else if (isFunc(fRCP, i, scopes)) {
          cout << "is func" << endl;
        }
        else if (scopes->hasVar(fRCP[i])) {
          cout << "has named" << endl;
        }
        else {
          int j = 1;
          for (; (fRCP[i - j]->types[0] == statement::op) && ((fRCP[i - j]->types[1] == op::mult) || (fRCP[i - j]->types[1] == op::amp)); j++) {}
          // if (scopes->hasVar(fRCP[i - j])) {
          //   cout << "has named else" << endl;
          //   statement* temp = new statement();
          //   temp->codeString = fRCP[i]->codeString;
          //   temp->types[0] = statement::datum;
          //   temp->layer = fRCP[i]->layer;
          //   temp->daType = datum::var;
          //   temp->dType = scopes->setType(fRCP[i - j]);
          //   delete fRCP[i];
          //   fRCP[i] = temp;
          //
          // }
          if (fRCP[i - j]->types[0] == statement::typeDecl) {
            statement* temp = new statement();
            temp->codeString = fRCP[i]->codeString;
            temp->types[0] = statement::datum;
            temp->layer = fRCP[i]->layer;
            temp->types.push_back(scopes->setType(fRCP[i - j]));  //TODO need to add types for pointers
            temp->types.push_back(datum::var);
            delete fRCP[i];
            fRCP[i] = temp;
          }
          else {
            cout << "is object decl" << endl;
            statement* temp = new statement();
            temp->codeString = fRCP[i]->codeString;
            temp->types[0] = statement::typeDecl;
            temp->layer = fRCP[i]->layer;
            temp->types.push_back(typeDecl::objectDecl);
            delete fRCP[i];
            fRCP[i] = temp;
            scopes->setType(fRCP[i]);
          }
        }

      }
      else if (fRCP[i]->types[0] == statement::undefKeyChar || fRCP[i]->types[0] == statement::bracket) {
        if (isPrecomp(fRCP[i])) {
          cout << "isPrecomp" << endl;
        }
        else if (is2CharKey(fRCP[i - 1], fRCP[i])) {
          cout << "is2CharKey" << endl;
        }
        else if (isOperator(fRCP[i])) {
          cout << "isOperator" << endl;
        }
        else if (isBracket(fRCP[i])) {
          cout << "finalizeExpressions | " << toString(fRCP[i]->codeString) << "is bracket" << endl;
          if (fRCP[i]->types[2] == bracket::opening) {
            if (fRCP[i]->types[1] == bracket::brace) {
				curScope->statements.push_back(fRCP[i]);
              curScope = scopes->newScope(curScope);
              cout << "new scope: ";
              //scopeTree::printScope(curScope);
            }
            else if (fRCP[i]->types[1] == bracket::round) {
              bracketLevel++;
              // if ((fRCP[i - 1]->types[0] == statement::undefName) || (fRCP[i - 1]->types[0] == statement::datum)) {
              //   func* temp = new func();
              //   temp->codeString = fRCP[i - 1]->codeString;
              //   temp->types[0] = statement::func;
              //   temp->layer = fRCP[i - 1]->layer;
              //   delete fRCP[i - 1];
              //   fRCP[i - 1] = temp;
              //
              //   if (fRCP[i - 2]->types[0] == statement::undefName) {
              //     typeDecl* temp2 = new typeDecl();
              //     temp2->codeString = fRCP[i - 2]->codeString;
              //     temp2->types[0] = statement::typeDecl;
              //     delete fRCP[i - 2];
              //     fRCP[i - 2] = temp2;
              //     cout << "abc" << endl;
              //     ((func*)fRCP[i - 1])->retType = scopes->setType(fRCP[i - 2]);
              //     cout << "def" << endl;
              //   }
              // }
            }
            else {
              bracketLevel++;
            }
          }
          else if (fRCP[i]->types[1] == bracket::brace) {
			 
            curScope = scopes->parentScope(curScope);
			fRCP[i]->layer = curScope;
            cout << "parent scope ";
            //scopeTree::printScope(curScope);
          }
          else {
            bracketLevel--;
          }
        }
      }
	  if (!((fRCP[i]->types[0] == statement::bracket) && (fRCP[i]->types[1] == bracket::brace) && (fRCP[i]->types[2] == bracket::opening))) {
		  curScope->statements.push_back(fRCP[i]);
	  }
    }
  }

  //generates operator trees;
  /*
  void secondRunCodeParse(vector<statement*>& sRCP) {
    //vector<genStatement*> parsedCode;
    //scopeTree* scopes = new scopeTree();
    statement* curExpr = new statement();
    curExpr->types[0] = statement::expr;
    bool bracketStatement = false;
    int bracketLevel = 0;
    for (int i = 0; i < sRCP.size();) {
      for (; (!bracketStatent && (sRCP[i]->types[0] != statement::semiCol)) || (bracketStatement && (bracketLevel != 0)); i++) {

      }

    }
  }
  */
  vector<statement*> secondRunCodeParse(vector<statement*>& sRCP) {
    int temp;
    return secondRunCodeParse(sRCP, 0, temp)->statements;
  }

  statement* secondRunCodeParse(vector<statement*>& sRCP, int i, int& jmp) {
    jmp = i;
    statement* curExpr = new statement();
    statement* localExprs = new statement();
    cout << "secondRunCodeParse | new local expression" << endl;
    localExprs->types[0] = statement::expr;
    curExpr->types[0] = statement::expr;
    bool bracketStatement = false;
    int bracketLevel = 0;
    int startBracketLevel = 0;
    for (; i < sRCP.size(); i++) {
      cout << "secondRunCodeParse | sRCP[" << i << "]: " << toString(sRCP[i]->codeString) << endl;
      if (sRCP[i]->types[0] == statement::precomp) {
        curExpr->statements.push_back(sRCP[i]);
        localExprs->statements.push_back(curExpr);
        curExpr = new statement();
        curExpr->types[0] = statement::expr;
      }
      else if ((sRCP[i]->types[0] == statement::branch) && ((sRCP[i]->types[1] == branch::ifBranch) || (sRCP[i]->types[1] == branch::elseIf))) {
        curExpr->statements.push_back(sRCP[i]);
        i++;
        startBracketLevel = bracketLevel;
        bracketLevel++;
        curExpr->statements.push_back(sRCP[i]);
        i++;
        while (bracketLevel != startBracketLevel) {
          if ((sRCP[i]->types[0] == statement::bracket) && (sRCP[i]->types[2] == bracket::opening)) {
            bracketLevel++;
          }
          else if ((sRCP[i]->types[0] == statement::bracket) && (sRCP[i]->types[2] == bracket::closing)) {
            bracketLevel--;
          }
          curExpr->statements.push_back(sRCP[i]);
          i++;
        }
        curExpr->statements.push_back(sRCP[i]);
        i++;
        int jump;
        curExpr->statements.push_back(secondRunCodeParse(sRCP, i, jump));
        i += jump;
		curExpr->statements.push_back(sRCP[i]);
        localExprs->statements.push_back(curExpr);
        curExpr = new statement();
        curExpr->types[0] = statement::expr;
      }
	  else if ((sRCP[i]->types[0] == statement::branch) && (sRCP[i]->types[1] == branch::elseBranch)) {
		  curExpr->statements.push_back(sRCP[i]);
		  i++;
		  curExpr->statements.push_back(sRCP[i]);
		  i++;
		  int jump;
		  curExpr->statements.push_back(secondRunCodeParse(sRCP, i, jump));
		  i += jump;
		  curExpr->statements.push_back(sRCP[i]);
		  localExprs->statements.push_back(curExpr);
		  curExpr = new statement();
		  curExpr->types[0] = statement::expr;
	  }
      // for (; (!bracketStatement && (sRCP[i]->types[0] != statement::semiCol)) || (bracketStatement && (bracketLevel != 0)); i++) {
      //
      // }
      else if ((sRCP[i]->types[0] == statement::loop) && ((sRCP[i]->types[1] == loop::whileLoop) || (sRCP[i]->types[1] == loop::forLoop))) {
        curExpr->statements.push_back(sRCP[i]);
        i++;
        startBracketLevel = bracketLevel;
        bracketLevel++;
        curExpr->statements.push_back(sRCP[i]);
        i++;
        while (bracketLevel != startBracketLevel) {
          if ((sRCP[i]->types[0] == statement::bracket) && (sRCP[i]->types[2] == bracket::opening)) {
            bracketLevel++;
          }
          else if ((sRCP[i]->types[0] == statement::bracket) && (sRCP[i]->types[2] == bracket::closing)) {
            bracketLevel--;
          }
          curExpr->statements.push_back(sRCP[i]);
          i++;
        }
        curExpr->statements.push_back(sRCP[i]);
        i++;
        int jump;
        curExpr->statements.push_back(secondRunCodeParse(sRCP, i, jump));
        i += jump;
		curExpr->statements.push_back(sRCP[i]);
        localExprs->statements.push_back(curExpr);
        curExpr = new statement();
        curExpr->types[0] = statement::expr;
      }
      else if ((sRCP[i]->types[0] == statement::typeDef) && (sRCP[i]->types[1] == typeDef::funcDef)) {
        curExpr->statements.push_back(sRCP[i]);
        i++;
        startBracketLevel = bracketLevel;
        bracketLevel++;
        curExpr->statements.push_back(sRCP[i]);
        i++;
        while (bracketLevel != startBracketLevel) {
          if ((sRCP[i]->types[0] == statement::bracket) && (sRCP[i]->types[2] == bracket::opening)) {
            bracketLevel++;
          }
          else if ((sRCP[i]->types[0] == statement::bracket) && (sRCP[i]->types[2] == bracket::closing)) {
            bracketLevel--;
          }
          curExpr->statements.push_back(sRCP[i]);
          i++;
        }
        curExpr->statements.push_back(sRCP[i]);
        i++;
        int jump;
        curExpr->statements.push_back(secondRunCodeParse(sRCP, i, jump));
        i += jump;
		curExpr->statements.push_back(sRCP[i]);
        localExprs->statements.push_back(curExpr);
        curExpr = new statement();
        curExpr->types[0] = statement::expr;
      }
      else if ((sRCP[i]->types[0] == statement::bracket) && (sRCP[i]->types[1] == bracket::brace) && (sRCP[i]->types[2] == bracket::opening)) {
        cout << "secondRunCodeParse | opening bracket" << endl;
        bracketLevel++;
        curExpr->statements.push_back(sRCP[i]);
        if (curExpr->statements.size() == 1) {
          bracketLevel--;
          cout << "secondRunCodeParse | empty open brace" << endl;
          i++;
          int jump;
          curExpr->statements.push_back(secondRunCodeParse(sRCP, i, jump));
          i += jump;
		  curExpr->statements.push_back(sRCP[i]);
          localExprs->statements.push_back(curExpr);
          curExpr = new statement();
          curExpr->types[0] = statement::expr;
        }
      }
      else if ((sRCP[i]->types[0] == statement::bracket) && (sRCP[i]->types[1] == bracket::brace) && (sRCP[i]->types[2] == bracket::closing)) {
        cout << "secondRunCodeParse | closing brace" << endl;
        bracketLevel--;
        cout << "secondRunCodeParse | bracketLevel: " << bracketLevel << endl;
        
        if (bracketLevel < 0) {
          cout << "secondRunCodeParse | returning to parent" << endl;
          break;
        }
		else {
			curExpr->statements.push_back(sRCP[i]);
		}
      }
      else if (sRCP[i]->types[0] == statement::semiCol) {
        cout << "secondRunCodeParse | adding curExpr to localExprs" << endl;
        curExpr->statements.push_back(sRCP[i]);
        localExprs->statements.push_back(curExpr);
        curExpr = new statement();
        curExpr->types[0] = statement::expr;
      }
      else {
        cout << "secondRunCodeParse | adding " << toString(sRCP[i]->codeString) << " to curExpr" << endl;
        curExpr->statements.push_back(sRCP[i]);
      }
    }
    jmp = i - jmp;
    if (!(curExpr->statements.empty())) {
      localExprs->statements.push_back(curExpr);
    }
    return localExprs;
  }

  //int n = 5 * 8 + 3 * 7
  //if (n == 3) {
    //int x = 4
  //}
  //for (int i = 0; i < 3; i++)
  bool isNumber(char c) {
    for (int i = 0; i < 10; i++) {
      if ((int)c == 48 + i) {
        return true;
      }
    }
    return false;
  }
};

class functionTracker {
public:
  int nextName = 0; //the next name to be given to a function
  vector<function*> functions;
  vector<char> nSpace;

  functionTracker() {
    //TODO: generate unique namespace
    nSpace.push_back('g');
    nSpace.push_back('e');
    nSpace.push_back('n');
    nSpace.push_back('F');
    nSpace.push_back('u');
    nSpace.push_back('n');
    nSpace.push_back('c');
    nSpace.push_back('s');
  }

  // function* newFunc() {
  //   function* retFunc = new function(this);
  //   functions.push_back(retFunc);
  //   return retFunc;
  // }

  statement* genNamespace() {
	  statement* state = new statement();
	  state->codeString.push_back('n');
	  state->codeString.push_back('a');
	  state->codeString.push_back('m');
	  state->codeString.push_back('e');
	  state->codeString.push_back('s');
	  state->codeString.push_back('p');
	  state->codeString.push_back('a');
	  state->codeString.push_back('c');
	  state->codeString.push_back('e');
	  state->codeString.push_back(' ');
	  for (char& c : nSpace) {
		  state->codeString.push_back(c);
	  }
	  state->codeString.push_back(' ');
	  state->codeString.push_back('{');
	  return state;
  }
};

class function {
public:
  int name;
  vector<statement*> expressions;
  bool returns = false;
  bool returnsVal = false;
  int retType;
  string retTypeName;

  function(functionTracker* funcTrack) {
    name = funcTrack->nextName++;
    funcTrack->functions.push_back(this);
  }

  statement* genCall(functionTracker* funcTrack, function* callingFunc) {
    statement* func = new statement();
	func->codeString.push_back('s');
	func->codeString.push_back('t');
	func->codeString.push_back('d');
	func->codeString.push_back(':');
	func->codeString.push_back(':');
	func->codeString.push_back('t');
	func->codeString.push_back('h');
	func->codeString.push_back('r');
	func->codeString.push_back('e');
	func->codeString.push_back('a');
	func->codeString.push_back('d');
	func->codeString.push_back(' ');
	func->codeString.push_back('f');
	string nameS = to_string(name);
	for (int i = 0; i < nameS.length(); i++) {
		func->codeString.push_back(nameS[i]);
	}
	func->codeString.push_back('(');
    for (char& c : funcTrack->nSpace) {
      func->codeString.push_back(c);
    }
    func->codeString.push_back(':');
    func->codeString.push_back(':');
    func->codeString.push_back('f');
    for (int i = 0; i < nameS.length(); i++) {
      func->codeString.push_back(nameS[i]);
    }
    //func->codeString.push_back('(');
    
	if (returnsVal) {
		if (callingFunc == NULL) {
			func->codeString.push_back(',');
			func->codeString.push_back(' ');
			func->codeString.push_back('f');
			for (int i = 0; i < nameS.length(); i++) {
				func->codeString.push_back(nameS[i]);
			}
			func->codeString.push_back('r');
		}
		else {
			func->codeString.push_back(',');
			func->codeString.push_back(' ');
			func->codeString.push_back('r');
			func->codeString.push_back('e');
			func->codeString.push_back('t');
		}
	}
	func->codeString.push_back(')');
	func->codeString.push_back(';');
    return func;
  }

  statement* genDecl() {
    statement* func = new statement();
    func->codeString.push_back('v');
    func->codeString.push_back('o');
    func->codeString.push_back('i');
    func->codeString.push_back('d');
    func->codeString.push_back(' ');
	func->codeString.push_back('f');
    string nameS = to_string(name);
    for (int i = 0; i < nameS.length(); i++) {
      func->codeString.push_back(nameS[i]);
    }
    func->codeString.push_back('(');
	if (returnsVal) {
		func->codeString.push_back(' ');
		for (int i = 0; i < retTypeName.length(); i++) {
			func->codeString.push_back(retTypeName[i]);
		}
		func->codeString.push_back('*');
		func->codeString.push_back(' ');
		func->codeString.push_back('r');
		func->codeString.push_back('e');
		func->codeString.push_back('t');
	}
    func->codeString.push_back(')');
    func->codeString.push_back(';');
    return func;
  }

  statement* genDef() {
    statement* func = new statement();
    func->codeString.push_back('v');
    func->codeString.push_back('o');
    func->codeString.push_back('i');
    func->codeString.push_back('d');
    func->codeString.push_back(' ');
	func->codeString.push_back('f');
    string nameS = to_string(name);
    for (int i = 0; i < nameS.length(); i++) {
      func->codeString.push_back(nameS[i]);
    }
    func->codeString.push_back('(');
	if (returnsVal) {
		func->codeString.push_back(' ');
		for (int i = 0; i < retTypeName.length(); i++) {
			func->codeString.push_back(retTypeName[i]);
		}
		func->codeString.push_back('*');
		func->codeString.push_back(' ');
		func->codeString.push_back('r');
		func->codeString.push_back('e');
		func->codeString.push_back('t');
	}
    func->codeString.push_back(')');
    func->codeString.push_back(' ');
    func->codeString.push_back('{');
    return func;
  }

  statement* genJoin() {
	  statement* func = new statement();
	  func->codeString.push_back('f');
	  string nameS = to_string(name);
	  for (int i = 0; i < nameS.length(); i++) {
		  func->codeString.push_back(nameS[i]);
	  }
	  func->codeString.push_back('.');
	  func->codeString.push_back('j');
	  func->codeString.push_back('o');
	  func->codeString.push_back('i');
	  func->codeString.push_back('n');
	  func->codeString.push_back('(');
	  func->codeString.push_back(')');
	  func->codeString.push_back(';');

	  return func;
  }
};

int main(int argc, char** argv) {
  loopKeys.push_back("for");
  loopKeys.push_back("while");
  loopKeys.push_back("do");

  wordKeys.push_back("include");
  wordKeys.push_back("using");
  wordKeys.push_back("namespace");
  wordKeys.push_back("new");
  wordKeys.push_back("delete");
  wordKeys.push_back("return");
  wordKeys.push_back("define");

  string fileName = string(argv[1]);
  //const char* fileName = "test.cpp";
  document file (fileName);

  file.printDocument();

  dependGraph* d = new dependGraph();
  scopeTree* scopes;
  nodeContainer* container = d->buildDependGraph(file, scopes);

  vector<statement*> finalExpressions = createAndInjectFuncs(container, scopes);

  document::writeDocument(fileName, finalExpressions);

  delete d;

}

bool isKeyChar(char c) {
  bool ret = false;
  for (int i = 0; i < sizeof(keyChars) / sizeof(char); i++) {
    if (c == keyChars[i]) {
      ret = true;
    }
  }
  return ret;
}

bool isLoop(statement*& state) {
  bool ret = false;
  // for (int i = 0; i < loopKeys.size(); i++) {
  //   if (loopKeys[i].equals(state->codeString)) {
  //     ret = true;
  //     statement* temp = new
  //   }
  // }
  if (areEqual(state->codeString, string(loopKeys[0]))) {
    ret = true;
    statement* temp = new statement();
    temp->types[0] = statement::loop;
    temp->codeString = state->codeString;
    temp->types.push_back(loop::forLoop);
    temp->layer = state->layer;
    temp->nStatements = 2;
    delete state;
    state = temp;
  }
  else if (areEqual(state->codeString, string(loopKeys[1]))) {
    ret = true;
    statement* temp = new statement();
    temp->types[0] = statement::loop;
    temp->codeString = state->codeString;
    temp->types.push_back(loop::whileLoop);
    temp->layer = state->layer;
    temp->nStatements = 2;
    delete state;
    state = temp;
  }
  else if (areEqual(state->codeString, string(loopKeys[2]))) {
    ret = true;
    statement* temp = new statement();
    temp->types[0] = statement::loop;
    temp->codeString = state->codeString;
    temp->types.push_back(loop::doLoop);
    temp->layer = state->layer;
    temp->nStatements = 2;
    delete state;
    state = temp;
  }
  return ret;
}

bool isBracket(statement*& state) {
  bool ret = false;
  if (state->codeString[0] == '(') {
    ret = true;
    statement* temp = new statement();
    temp->codeString = state->codeString;
    temp->types[0] = statement::bracket;
    temp->types.push_back(bracket::round);
    temp->types.push_back(bracket::opening);
    temp->layer = state->layer;
    temp->nStatements = 1;
    delete state;
    state = temp;
  }
  else if (state->codeString[0] == '{') {
    ret = true;
    statement* temp = new statement();
    temp->codeString = state->codeString;
    temp->types[0] = statement::bracket;
    temp->types.push_back(bracket::brace);
    temp->types.push_back(bracket::opening);
    temp->layer = state->layer;
    temp->nStatements = 1;
    delete state;
    state = temp;
  }
  else if (state->codeString[0] == '[') {
    ret = true;
    statement* temp = new statement();
    temp->codeString = state->codeString;
    temp->types[0] = statement::bracket;
    temp->types.push_back(bracket::square);
    temp->types.push_back(bracket::opening);
    temp->layer = state->layer;
    temp->nStatements = 1;
    delete state;
    state = temp;
  }
  else if (state->codeString[0] == ')') {
    ret = true;
    statement* temp = new statement();
    temp->codeString = state->codeString;
    temp->types[0] = statement::bracket;
    temp->types.push_back(bracket::round);
    temp->types.push_back(bracket::closing);
    temp->layer = state->layer;
    delete state;
    state = temp;
  }
  else if (state->codeString[0] == '}') {
    ret = true;
    statement* temp = new statement();
    temp->codeString = state->codeString;
    temp->types[0] = statement::bracket;
    temp->types.push_back(bracket::brace);
    temp->types.push_back(bracket::closing);
    temp->layer = state->layer;
    delete state;
    state = temp;
  }
  else if (state->codeString[0] == ']') {
    ret = true;
    statement* temp = new statement();
    temp->codeString = state->codeString;
    temp->types[0] = statement::bracket;
    temp->types.push_back(bracket::square);
    temp->types.push_back(bracket::closing);
    temp->layer = state->layer;
    delete state;
    state = temp;
  }
  return ret;
}

bool isWhiteSpace(char c) {
  bool ret = false;
  if ((c == ' ') || (c == '\n') || (c == '\t') || ((int)c == 0)) {
    ret = true;
  }
  return ret;
}

bool isPrecomp(statement*& state) {
  bool ret = false;
  if (areEqual(state->codeString, '#')) {
    ret = true;
    statement* temp = new statement();
    temp->codeString = state->codeString;
    temp->types[0] = statement::precomp;
    temp->layer = state->layer;
    delete state;
    state = temp;
  }
  return ret;
}

bool is2CharKey(statement*& state1, statement*& state2) {
  bool ret = false;
  if ((state1->types[0] == statement::op) && (state2->types[0] == statement::op)) {
    char name[] = {state1->codeString[0], state2->codeString[0]};
    if ((name[0] == '+') && (name[1] == '=')) {

    }
  }
  return ret;
}

bool isOperator(statement*& state) {
  bool ret = false;
  if (areEqual(state->codeString, '=')) {
    ret = true;
    statement* temp = new statement();
    temp->codeString = state->codeString;
    temp->types[0] = statement::op;
    temp->types.push_back(op::assign);
    temp->layer = state->layer;
    temp->nStatements = 2;
    delete state;
    state = temp;
  }
  else if (areEqual(state->codeString, '+')) {
    ret = true;
    statement* temp = new statement();
    temp->codeString = state->codeString;
    temp->types[0] = statement::op;
    temp->types.push_back(op::add);
    temp->layer = state->layer;
    temp->nStatements = 2;
    delete state;
    state = temp;
  }
  else if (areEqual(state->codeString, '-')) {
    ret = true;
    statement* temp = new statement();
    temp->codeString = state->codeString;
    temp->types[0] = statement::op;
    temp->types.push_back(op::sub);
    temp->layer = state->layer;
    temp->nStatements = 2;
    delete state;
    state = temp;
  }
  else if (areEqual(state->codeString, '*')) {
    ret = true;
    statement* temp = new statement();
    temp->codeString = state->codeString;
    temp->types[0] = statement::op;
    temp->types.push_back(op::mult);
    temp->layer = state->layer;
    delete state;
    state = temp;
  }
  else if (areEqual(state->codeString, '/')) {
    ret = true;
    statement* temp = new statement();
    temp->codeString = state->codeString;
    temp->types[0] = statement::op;
    temp->types.push_back(op::div);
    temp->layer = state->layer;
    temp->nStatements = 2;
    delete state;
    state = temp;
  }
  return ret;
}

bool areEqual(vector<char>& s1, vector<char>& s2) {
  //cout << "areEqual(vector, vector)" << endl;
  if (s1.size() != s2.size()) {
    return false;
  }
  for (int i = 0; i < s1.size(); i++) {
    if (s1[i] != s2[i]) {
      return false;
    }
  }
  return true;
}

bool areEqual(vector<char>& s1, char* s2) {
  //cout << "areEqual(vector, char*)" << endl;
  string temp(s2);
  return areEqual(s1, temp);
  // for (int i = 0; i < s1.size(); i++) {
  //   cout << "s1[" << i << "] = " << s1[i] << " | s2[" << i << "] = " << s2[i] << endl;
  //   if (s1[i] != s2[i]) {
  //     return false;
  //   }
  // }
  // return true;
}

bool isKeyWord(statement*& state) {
  bool ret = false;
  for (int i = 0; i < wordKeys.size(); i++) {
    if (areEqual(state->codeString, string(wordKeys[i]))) {
      ret = true;
      statement* temp = new statement();
      temp->codeString = state->codeString;
      temp->types[0] = statement::keyWord;
      temp->layer = state->layer;
      temp->nStatements = 1;
      delete state;
      state = temp;
    }
  }
  return ret;
}

bool areEqual(vector<char>& s1, string s2) {
  //cout << "areEqual(vector, string)" << endl;
  //cout << toString(s1) << " __ " << s2 << endl;
  //cout << "s1 size: " << s1.size() << " | s2 size: " << s2.size() << endl;
  if (s2.size() != s1.size()) {
    return false;
  }
  for (int i = 0; i < s1.size(); i++) {
    if (s1[i] != s2[i]) {
      return false;
    }
  }
  return true;
}

bool areEqual(vector<char>& s1, char c) {
  //cout << "areEqual(vector, char)" << endl;
  if ((s1.size() > 1) || (s1[0] != c)) {
    return false;
  }
  return true;
}

string toString(vector<char>& s) {
  string ret = "";
  for (int i = 0; i < s.size(); i++) {
    ret += s[i];
  }
  return ret;
}

bool isBranch(statement*& state) {
  bool ret = false;
  if (areEqual(state->codeString, "if")) {
	  ret = true;
	  statement* temp = new statement();
	  temp->codeString = state->codeString;
	  temp->types[0] = statement::branch;
	  temp->types.push_back(branch::ifBranch);
	  temp->layer = state->layer;
	  temp->nStatements = 1;
	  delete state;
	  state = temp;
  }
  if (areEqual(state->codeString, "else")) {
	  ret = true;
	  statement* temp = new statement();
	  temp->codeString = state->codeString;
	  temp->types[0] = statement::branch;
	  temp->types.push_back(branch::elseBranch);
	  temp->layer = state->layer;
	  temp->nStatements = 1;
	  delete state;
	  state = temp;
  }
  /*
  if (areEqual(state->codeString, "else if")) {
	  statement* temp = new statement();
	  temp->codeString = state->codeString;
	  temp->types[0] = statement::branch;
	  temp->types.push_back(branch::ifBranch);
	  temp->layer = state->layer;
	  temp->nStatements = 1;
	  delete state;
	  state = temp;
  }
  */
  return ret;
}

// void setType(statement*& state) {
//
// }

// bool isObjectDecl(statement*& state) {
//   if (areEqual(state->codeString, "class")) {
//     classDecl* temp = new classDecl();
//     temp->codeString = state->codeString;
//     temp->types[0] = statement::objectDecl;
//     temp->layer = state->layer;
//     temp->declType = objectDecl::classDecl;
//     delete state;
//     state = temp;
//     return true;
//   }
//   else if (areEqual(state->codeString, "struct")) {
//     structDecl* temp = new structDecl();
//     temp->codeString = state->codeString;
//     temp->types[0] = statement::objectDecl;
//     temp->layer = state->layer;
//     temp->declType = objectDecl::structDecl;
//     delete state;
//     state = temp;
//     return true;
//   }
//   return false;
// }

bool isClassDecl(statement*& state, statement*& state2) {
  cout << "isClassDecl start" << endl;
  if (areEqual(state->codeString, string("class"))) {
    if (state2->types[0] == statement::semiCol) {
      statement* temp = new statement();
      temp->codeString = state->codeString;
      temp->types[0] = statement::typeDecl;
      temp->layer = state->layer;
      temp->types.push_back(typeDecl::classDecl);
      temp->nStatements = 1;
      delete state;
      state = temp;
      return true;
    }
    else {
      statement* temp = new statement();
      temp->codeString = state->codeString;
      temp->types[0] = statement::typeDef;
      temp->layer = state->layer;
      temp->types.push_back(typeDef::classDef);
      temp->nStatements = 1;
      delete state;
      state = temp;
      return true;
    }
  }
  return false;
}

// bool isFunc(statement*& state1, statement*& state2) {
//   if (isBracket(state2)) {
//     if ((state2->types[2]) && (state2->types[1] == bracket::round)) {
//       statement* temp = new statement();
//       temp->codeString = state1->codeString;
//       temp->types[0] = statement::func;
//       temp->layer = state1->layer;
//       temp->nStatements = 1;
//       delete state1;
//       state1 = temp;
//       return true;
//     }
//   }
//   return false;
// }

bool isFunc(vector<statement*>& statements, int n, scopeTree* scopes) {
  cout << "isFunc start" << endl;
  int j = 1;
  for (; (statements[n - j]->types[0] == statement::op) && ((statements[n - j]->types[1] == op::mult) || (statements[n - j]->types[1] == op::amp)); j++) {}
  if (statements[n - j]->types[0] == statement::typeDecl) {
	  int k = n - j;
    cout << "isFunc | is typeDecl" << endl;
    if (isBracket(statements[n + 1])) {
      if ((statements[n + 1]->types[2] == bracket::opening) && (statements[n + 1]->types[1] == bracket::round)) {
        int bracketLevel = 1;
        for (j = 2; (n + j < statements.size()) && (bracketLevel > 0); j++) {
          if (isBracket(statements[n + j])) {
            if (statements[n + j]->types[2] == bracket::opening) {
              bracketLevel++;
            }
            else if (statements[n + j]->types[2] == bracket::closing) {
              bracketLevel--;
            }
          }
        }
        if (statements[n + j]->types[0] == statement::semiCol) {
          statement* temp = new statement();
          temp->codeString = statements[n]->codeString;
          temp->types[0] = statement::typeDecl;
          temp->types.push_back(typeDecl::funcDecl);
          temp->layer = statements[n]->layer;
          temp->nStatements = 1;
          delete statements[n];
          statements[n] = temp;
          return true;
        }
        else if (isBracket(statements[n + j]) && (statements[n + j]->types[1] == bracket::brace) && (statements[n + j]->types[2] == bracket::opening)) {
          statement* temp = new statement();
          temp->codeString = statements[n]->codeString;
          temp->types[0] = statement::typeDef;
          temp->types.push_back(typeDef::funcDef);
		  temp->types.push_back(scopes->setType(statements[k]));
          temp->layer = statements[n]->layer;
          temp->nStatements = 1;
          delete statements[n];
          statements[n] = temp;
          return true;
        }
      }
    }
  }
  else if (isBracket(statements[n + 1])) {
    if ((statements[n + 1]->types[2] == bracket::opening) && (statements[n + 1]->types[1] == bracket::round)) {
      statement* temp = new statement();
      temp->codeString = statements[n]->codeString;
      temp->types[0] = statement::funcCall;
      temp->layer = statements[n]->layer;
      temp->nStatements = 1;
      delete statements[n];
      statements[n] = temp;
      return true;
    }
  }
  return false;
}

void printExpressions(vector<statement*>& expressions, int* i) {
  int thisExpr = 0;
  for (statement*& state : expressions) {
    thisExpr = *i;
    if (state->types[0] == statement::expr) {
      (*i)++;
      cout << "start of expression " << thisExpr << endl;
      printExpressions(state->statements, i);
      cout << endl;
      cout << "end of expression " << thisExpr << endl;
    }
    else {
      cout << toString(state->codeString) << " ";
    }
  }
}

void printExpressions(vector<statement*>& expressions) {
  for (statement*& state : expressions) {
    if (state->types[0] == statement::expr) {
      cout << "start of expression "<< endl;
      printExpressions(state->statements);
      cout << endl;
      cout << "end of expression " << endl;
    }
    else {
      cout << toString(state->codeString) << " ";
    }
  }
}

vector<statement*> createAndInjectFuncs(nodeContainer* container, scopeTree* scopes) {
  cout << "createAndInjectFuncs" << endl;
  functionTracker* funcTrack = new functionTracker();
  vector<statement*> finalExpressions;
  cout << "creating funcs" << endl;
  createFuncsFromGraphs(container, false, finalExpressions, funcTrack, 0, scopes, 0, NULL);
  cout << "inserting funcs" << endl;
  vector<statement*>::iterator it = finalExpressions.begin();
  for (int i = 0; i < finalExpressions.size(); i++) {
    bool isInclude = false;
    for (int j = 0; j < finalExpressions[i]->statements.size(); j++) {
      if (finalExpressions[i]->statements[j]->types[0] == statement::precomp) {
        string s = "include";
        isInclude = true;
        for (int k = 1; (k < finalExpressions[i]->statements[j]->codeString.size()) && (k - 1 < s.length()); k++) {
          if (finalExpressions[i]->statements[j]->codeString[k] != s[k - 1]) {
            isInclude = false;
          }
        }
      }
    }
    if (!isInclude) {
      it = it + i;
	  statement* tempExpr3 = new statement();
	  statement* include = new statement();
	  include->codeString.push_back('#');
	  include->codeString.push_back('i');
	  include->codeString.push_back('n');
	  include->codeString.push_back('c');
	  include->codeString.push_back('l');
	  include->codeString.push_back('u');
	  include->codeString.push_back('d');
	  include->codeString.push_back('e');
	  include->codeString.push_back(' ');
	  include->codeString.push_back('<');
	  include->codeString.push_back('t');
	  include->codeString.push_back('h');
	  include->codeString.push_back('r');
	  include->codeString.push_back('e');
	  include->codeString.push_back('a');
	  include->codeString.push_back('d');
	  include->codeString.push_back('>');
	  tempExpr3->statements.push_back(include);
	  it = finalExpressions.insert(it, tempExpr3) + 1;
	  tempExpr3 = new statement();
	  tempExpr3->statements.push_back(funcTrack->genNamespace());
	  it = finalExpressions.insert(it, tempExpr3) + 1;
      for (function*& func : funcTrack->functions) {
		  statement* tempExpr = new statement();
		  tempExpr->statements.push_back(func->genDecl());
        it = finalExpressions.insert(it, tempExpr) + 1;
      }

      for (function*& func : funcTrack->functions) {
		  statement* tempExpr = new statement();
		  tempExpr->statements.push_back(func->genDef());
        it = finalExpressions.insert(it, tempExpr) + 1;
        for (statement*& expr : func->expressions) {
          it = finalExpressions.insert(it, expr) + 1;
        }
        statement* temp = new statement();
        temp->codeString.push_back('}');
		tempExpr = new statement();
		tempExpr->statements.push_back(temp);
        it = finalExpressions.insert(it, tempExpr) + 1;
      }
	  statement* temp = new statement();
	  temp->codeString.push_back('}');
	  tempExpr3 = new statement();
	  tempExpr3->statements.push_back(temp);
	  it = finalExpressions.insert(it, tempExpr3) + 1;

      break;
    }
  }
  
  for (statement*& expr : finalExpressions) {
	  for (statement* state : expr->statements) {
		  cout << toString(state->codeString) << " ";
	  }
	  cout << endl;
  }

  return finalExpressions;
}

void createFuncsFromGraphs(nodeContainer* container, bool insideFuncDef, vector<statement*>& finalExpressions, functionTracker* funcTrack, int ng, scopeTree* scopes, int retType, function* callingFunc) {
  if (insideFuncDef) {
    cout << "insideFuncDef" << endl;
	vector<function*> levelFuncs;
    for (int i = 0; i < container->nodeGraphs.size(); i++) {
      function* func = new function(funcTrack);
	  func->retType = retType;
	  func->retTypeName = scopes->dTypes[retType];
	  levelFuncs.push_back(func);
	  for (int n = 0; n < container->nodes.size(); n++) {
		  graphNode* node = container->nodes[n];
        bool partOfGraph = false;
        for (graphNode*& node2 : container->nodeGraphs[i]) {
          if (node2 == node) {
            partOfGraph = true;
          }
        }

        if (partOfGraph) {
			
          if (node->container != NULL) {
			  statement* expr = new statement();
			  for (int i = 0; i < node->expression->statements.size() - 1; i++) {
				  expr->statements.push_back(node->expression->statements[i]);
			  }
			  func->expressions.push_back(expr);
            createFuncsFromGraphs(node->container, insideFuncDef, func->expressions, funcTrack, i, scopes, retType, func);
			statement* expr2 = new statement();
			expr2->statements.push_back(node->expression->statements[node->expression->statements.size() - 1]);
			func->expressions.push_back(expr2);
          }
		  else {
			  if ((node->expression->statements[0]->types[0] == statement::keyWord) && (areEqual(node->expression->statements[0]->codeString, "return"))) {
				  func->returns = true;
				  
				  if (node->expression->statements.size() > 2) {
					  func->returnsVal = true;
					  statement* temp = new statement();
					  temp->types[0] = statement::funcRet;
					  statement* exprTemp = new statement();
					  /*
					  temp->codeString.push_back('r');
					  temp->codeString.push_back('e');
					  temp->codeString.push_back('t');
					  temp->codeString.push_back(' ');
					  temp->codeString.push_back('=');
					  temp->codeString.push_back(' ');
					  temp->codeString.push_back('n');
					  temp->codeString.push_back('e');
					  temp->codeString.push_back('w');
					  temp->codeString.push_back(' ');
					  for (int t = 0; t < func->retTypeName.length(); t++) {
						  temp->codeString.push_back(func->retTypeName[t]);
					  }
					  temp->codeString.push_back('(');
					  temp->codeString.push_back(')');
					  temp->codeString.push_back(';');
					  exprTemp->statements.push_back(temp);
					  func->expressions.push_back(exprTemp);
					  temp = new statement();
					  exprTemp = new statement();
					  */
					  temp->codeString.push_back('*');
					  temp->codeString.push_back('r');
					  temp->codeString.push_back('e');
					  temp->codeString.push_back('t');
					  temp->codeString.push_back(' ');
					  temp->codeString.push_back('=');
					  temp->codeString.push_back(' ');
					  for (int j = 1; j < node->expression->statements.size() - 1; j++) {
						  for (int k = 0; k < node->expression->statements[j]->codeString.size(); k++) {
							  temp->codeString.push_back(node->expression->statements[j]->codeString[k]);
						  }
						  temp->codeString.push_back(' ');
					  }
					  temp->codeString.push_back(';');
					  exprTemp->statements.push_back(temp);
					  func->expressions.push_back(exprTemp);

					  temp = new statement();
					  //temp->types[0] = statement::funcRet;
					  exprTemp = new statement();
					  temp->codeString.push_back('r');
					  temp->codeString.push_back('e');
					  temp->codeString.push_back('t');
					  temp->codeString.push_back('u');
					  temp->codeString.push_back('r');
					  temp->codeString.push_back('n');
					  temp->codeString.push_back(';');
					  exprTemp->statements.push_back(temp);
					  func->expressions.push_back(exprTemp);
				  }
			  }
			  else {
				  func->expressions.push_back(node->expression);
			  }
		  }
        }
      }
	  if (func->returns && func->returnsVal) {
		  
		  
		  if (callingFunc == NULL) {
			  statement* tempExpr = new statement();
			  statement* temp = new statement();
			  for (int t = 0; t < func->retTypeName.length(); t++) {
				  temp->codeString.push_back(func->retTypeName[t]);
			  }
			  temp->codeString.push_back('*');
			  temp->codeString.push_back(' ');
			  temp->codeString.push_back('f');
			  string nameS = to_string(func->name);
			  for (int j = 0; j < nameS.length(); j++) {
				  temp->codeString.push_back(nameS[j]);
			  }
			  temp->codeString.push_back('r');
			  temp->codeString.push_back(' ');
			  temp->codeString.push_back('=');
			  temp->codeString.push_back(' ');
			  temp->codeString.push_back('n');
			  temp->codeString.push_back('e');
			  temp->codeString.push_back('w');
			  temp->codeString.push_back(' ');
			  for (int t = 0; t < func->retTypeName.length(); t++) {
				  temp->codeString.push_back(func->retTypeName[t]);
			  }
			  temp->codeString.push_back('(');
			  temp->codeString.push_back(')');
			  temp->codeString.push_back(';');
			  tempExpr->statements.push_back(temp);
			  finalExpressions.push_back(tempExpr);
		  }
		  
	  }
	  statement* tempExpr = new statement();
	  tempExpr->statements.push_back(func->genCall(funcTrack, callingFunc));
	  finalExpressions.push_back(tempExpr);
    }
	for (function* f : levelFuncs) {
		statement* tempExpr = new statement();
		tempExpr->statements.push_back(f->genJoin());
		finalExpressions.push_back(tempExpr);
	}
	
	for (function* f : levelFuncs) {
		if (f->returns) {
			if (callingFunc != NULL) {
				callingFunc->returns = true;
			}
			/*
			statement* tempExpr = new statement();
			statement* tempState = new statement();
			tempState->types[0] = statement::keyWord;
			tempState->codeString.push_back('r');
			tempState->codeString.push_back('e');
			tempState->codeString.push_back('t');
			//tempState->codeString.push_back('u');
			//tempState->codeString.push_back('r');
			//tempState->codeString.push_back('n');
			statement* tempState2;
			if (f->returnsVal) {
				tempState2 = new statement();
				//tempState->codeString.push_back(' ');
				tempState2->codeString.push_back('f');
				string nameS = to_string(f->name);
				for (int j = 0; j < nameS.length(); j++) {
					tempState2->codeString.push_back(nameS[j]);
				}
				tempState2->codeString.push_back('r');
				
			}
			statement* tempState3 = new statement();
			tempState3->codeString.push_back(';');
			tempExpr->statements.push_back(tempState);
			if (f->returnsVal) {
				tempExpr->statements.push_back(tempState2);
			}
			tempExpr->statements.push_back(tempState3);
			//finalExpressions.push_back(tempExpr);
			graphNode* tempNode = new graphNode();
			tempNode->expression = tempExpr;
			//container->owner->owningContainer->nodeGraphs[ng].push_back(tempNode);
			//container->owner->owningContainer->nodes.push_back(tempNode);
			*/
			if (callingFunc == NULL) {
				statement* temp = new statement();
				//temp->types[0] = statement::funcRet;
				statement* exprTemp = new statement();
				for (int t = 0; t < f->retTypeName.length(); t++) {
					temp->codeString.push_back(f->retTypeName[t]);
				}
				temp->codeString.push_back(' ');
				temp->codeString.push_back('f');
				string nameS = to_string(f->name);
				for (int j = 0; j < nameS.length(); j++) {
					temp->codeString.push_back(nameS[j]);
				}
				temp->codeString.push_back('d');
				temp->codeString.push_back(' ');
				temp->codeString.push_back('=');
				temp->codeString.push_back(' ');
				temp->codeString.push_back('*');
				temp->codeString.push_back('f');
				for (int j = 0; j < nameS.length(); j++) {
					temp->codeString.push_back(nameS[j]);
				}
				temp->codeString.push_back('r');
				temp->codeString.push_back(';');
				exprTemp->statements.push_back(temp);
				finalExpressions.push_back(exprTemp);
				temp = new statement();
				exprTemp = new statement();
				temp->codeString.push_back('d');
				temp->codeString.push_back('e');
				temp->codeString.push_back('l');
				temp->codeString.push_back('e');
				temp->codeString.push_back('t');
				temp->codeString.push_back('e');
				temp->codeString.push_back(' ');
				temp->codeString.push_back('f');
				for (int j = 0; j < nameS.length(); j++) {
					temp->codeString.push_back(nameS[j]);
				}
				temp->codeString.push_back('r');
				temp->codeString.push_back(';');
				exprTemp->statements.push_back(temp);
				finalExpressions.push_back(exprTemp);
				temp = new statement();
				exprTemp = new statement();

				temp->codeString.push_back('r');
				temp->codeString.push_back('e');
				temp->codeString.push_back('t');
				temp->codeString.push_back('u');
				temp->codeString.push_back('r');
				temp->codeString.push_back('n');
				temp->codeString.push_back(' ');
				temp->codeString.push_back('f');
				for (int j = 0; j < nameS.length(); j++) {
					temp->codeString.push_back(nameS[j]);
				}
				temp->codeString.push_back('d');
				temp->codeString.push_back(';');
				exprTemp->statements.push_back(temp);
				finalExpressions.push_back(exprTemp);
			}
			else {
				statement* tempState4 = new statement();
				//tempState4->codeString.push_back('r');
				//tempState4->codeString.push_back('e');
				//tempState4->codeString.push_back('t');
				//tempState4->codeString.push_back(' ');
				//tempState4->codeString.push_back('=');
				//tempState4->codeString.push_back(' ');
				if (f->returnsVal) {
					if (callingFunc != NULL) {
						callingFunc->returnsVal = true;
					}
					//tempState4->codeString.push_back('f');
					//string nameS = to_string(f->name);
					//for (int j = 0; j < nameS.length(); j++) {
						//tempState4->codeString.push_back(nameS[j]);
					//}
					//tempState4->codeString.push_back('r');
				}
				//tempState4->codeString.push_back(';');

				statement* tempExpr = new statement();
				//tempExpr->statements.push_back(tempState4);
				//finalExpressions.push_back(tempExpr);


				//tempExpr = new statement();
				statement* tempState = new statement();
				tempState->types[0] = statement::keyWord;
				tempState->codeString.push_back('r');
				tempState->codeString.push_back('e');
				tempState->codeString.push_back('t');
				tempState->codeString.push_back('u');
				tempState->codeString.push_back('r');
				tempState->codeString.push_back('n');
				tempState->codeString.push_back(';');
				tempExpr->statements.push_back(tempState);
				finalExpressions.push_back(tempExpr);
			}
		}
	}
	
  }
  else {
    cout << "not inside func def" << endl;
	for (int n = 0; n < container->nodes.size(); n++) {
		graphNode* node = container->nodes[n];
      cout << "printing node expression" << endl;
      int* x = new int();
      *x = 0;
      printExpressions(node->expression->statements, x);
      delete x;
      cout << endl;
      cout << "done printing node expression" << endl;
      
      if (node->container != NULL) {
		  statement* expr = new statement();
		  for (int i = 0; i < node->expression->statements.size() - 1; i++) {
			  expr->statements.push_back(node->expression->statements[i]);
		  }
		  finalExpressions.push_back(expr);
        cout << "container not null" << endl;
        for (statement* state : node->expression->statements) {
          cout << toString(state->codeString) << endl;
          if ((state->types[0] == statement::typeDef) && (state->types[1] == typeDef::funcDef)) {
            createFuncsFromGraphs(node->container, true, finalExpressions, funcTrack, 0, scopes, state->types[2], NULL);
          }
        }
		statement* expr2 = new statement();
		expr2->statements.push_back(node->expression->statements[node->expression->statements.size() - 1]);
		finalExpressions.push_back(expr2);
      }
	  else {
		  finalExpressions.push_back(node->expression);
	  }
    }
  }
}
