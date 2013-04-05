//===--------------- JavaTypes.h - Java primitives ------------------------===//
//
//                          The VMKit project
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef JNJVM_JAVA_TYPES_H
#define JNJVM_JAVA_TYPES_H

#include "types.h"
#include <vector>

namespace j3 {

class CommonClass;
class ClassPrimitive;

#define VOID_ID 0
#define BOOL_ID 1
#define BYTE_ID 2
#define CHAR_ID 3
#define SHORT_ID 4
#define INT_ID 5
#define FLOAT_ID 6
#define LONG_ID 7
#define DOUBLE_ID 8
#define OBJECT_ID 9
#define ARRAY_ID 10
#define NUM_ASSESSORS 11

static const char I_TAB = '[';
static const char I_END_REF = ';';
static const char I_PARG = '(';
static const char I_PARD = ')';
static const char I_BYTE = 'B';
static const char I_CHAR = 'C';
static const char I_DOUBLE = 'D';
static const char I_FLOAT = 'F';
static const char I_INT = 'I';
static const char I_LONG = 'J';
static const char I_REF = 'L';
static const char I_SHORT = 'S';
static const char I_VOID = 'V';
static const char I_BOOL = 'Z';
static const char I_SEP = '/';

/// Typedef - Each class has a Typedef representation. A Typedef is also a class
/// which has not been loaded yet. Typedefs are hashed on the name of the class.
/// Hashing is for memory purposes, not for comparison.
///
class Typedef {
public:
  
  /// keyName - The name of the Typedef. It is the representation of a class
  /// in a Java signature, e.g. "Ljava/lang/Object;".
  ///
  const UTF8* keyName;
  
  /// assocClass - Given the loaded, try to load the class represented by this
  /// Typedef.
  ///
  virtual CommonClass* assocClass() const = 0;
  
  virtual CommonClass* findAssocClass() const {
    return assocClass();
  }

  /// trace - Does this type need to be traced by the GC?
  ///
  virtual bool trace() const = 0;
  
  /// isPrimitive - Is this type a primitive type?
  ///
  virtual bool isPrimitive() const {
    return false;
  }
  
  /// isReference - Is this type a reference type?
  ///
  virtual bool isReference() const {
    return true;
  }
  
  /// isUnsigned - Is this type unsigned?
  ///
  virtual bool isUnsigned() const {
    return false;
  }
  
  virtual bool isVoid() const {
    return false;
  }

  virtual bool isLong() const {
    return false;
  }

  virtual bool isInt() const {
    return false;
  }

  virtual bool isChar() const {
    return false;
  }

  virtual bool isShort() const {
    return false;
  }

  virtual bool isByte() const {
    return false;
  }

  virtual bool isBool() const {
    return false;
  }

  virtual bool isFloat() const {
    return false;
  }

  virtual bool isDouble() const {
    return false;
  }

  /// getName - Get the name of the type, i.e. java.lang.String or
  /// I.
  virtual const UTF8* getName() const {
    return keyName;
  }

  /// getKey - Get the name of the type, i.e. Ljava/lang/String; or
  /// I.
  const UTF8* getKey() const {
    return keyName;
  }

  virtual char getId() const = 0;
  
  virtual ~Typedef() {}
};

class PrimitiveTypedef : public Typedef {
private:
  ClassPrimitive* prim;
  bool unsign;
  char charId;
  
public:

  virtual char getId() const {
    return charId;
  }
  
  virtual bool trace() const {
    return false;
  }
  
  virtual bool isPrimitive() const {
    return true;
  }
  
  virtual bool isReference() const {
    return false;
  }

  virtual bool isUnsigned() const {
    return unsign;
  }

  virtual CommonClass* assocClass() const {
    return (CommonClass*)prim;
  }

  PrimitiveTypedef(const UTF8* name, ClassPrimitive* cl, bool u, char i) {
    keyName = name;
    prim = cl;
    unsign = u;
    charId = i;
  }
  
  virtual bool isVoid() const {
    return charId == I_VOID;
  }

  virtual bool isLong() const {
    return charId == I_LONG;
  }

  virtual bool isInt() const {
    return charId == I_INT;
  }

  virtual bool isChar() const {
    return charId == I_CHAR;
  }

  virtual bool isShort() const {
    return charId == I_SHORT;
  }

  virtual bool isByte() const {
    return charId == I_BYTE;
  }

  virtual bool isBool() const {
    return charId == I_BOOL;
  }

  virtual bool isFloat() const {
    return charId == I_FLOAT;
  }

  virtual bool isDouble() const {
    return charId == I_DOUBLE;
  }
  
};

class ArrayTypedef : public Typedef {
public:
  
  virtual bool trace() const {
    return true;
  }

  virtual CommonClass* assocClass() const;
  virtual CommonClass* findAssocClass() const;

  ArrayTypedef(const UTF8* name) {
    keyName = name;
  }

  virtual char getId() const {
    return I_REF;
  }
};

class ObjectTypedef : public Typedef {
private:
  /// pseudoAssocClassName - The real name of the class this Typedef
  /// represents, e.g. "java/lang/Object"
  ///
  const UTF8* pseudoAssocClassName;

public:
  virtual bool trace() const {
    return true;
  }
  
  virtual CommonClass* assocClass() const;
  virtual CommonClass* findAssocClass() const;

  ObjectTypedef(const UTF8*name, UTF8Map* map);
  
  virtual const UTF8* getName() const {
    return pseudoAssocClassName;
  }

  virtual char getId() const {
    return I_REF;
  }
};


/// Signdef - This class represents a Java signature. Each Java method has a
/// Java signature. Signdefs are hashed for memory purposes, not equality
/// purposes.
///
class Signdef {
private:
  
  /// _staticCallBuf - A dynamically generated method which calls a static Java
  /// function with the specific signature and receive the arguments in a
  /// buffer.
  ///
  word_t _staticCallBuf;
  word_t staticCallBuf();

  /// _virtualCallBuf - A dynamically generated method which calls a virtual
  /// Java function with the specific signature and receive the arguments in a
  /// buffer.
  ///
  word_t _virtualCallBuf;
  word_t virtualCallBuf();
  
  /// _staticCallAP - A dynamically generated method which calls a static Java
  /// function with the specific signature and receive the arguments in a
  /// variable argument handle.
  ///
  word_t _staticCallAP;
  word_t staticCallAP();
  
  /// _virtualCallBuf - A dynamically generated method which calls a virtual
  /// Java function with the specific signature and receive the arguments in a
  /// variable argument handle.
  ///
  word_t _virtualCallAP; 
  word_t virtualCallAP();
  
  word_t _virtualCallStub; 
  word_t virtualCallStub();
  
  word_t _specialCallStub;
  word_t specialCallStub();
  
  word_t _staticCallStub; 
  word_t staticCallStub();
  
public:

  /// initialLoader - The loader that first loaded this signdef.
  ///

  /// keyName - The Java name of the signature, e.g. "()V".
  ///
  const UTF8* keyName;
  
  /// nativeName - Get a native name for callbacks emitted AOT.
  ///
  void nativeName(char* buf, const char* ext) const;

  /// Signdef - Create a new Signdef.
  ///
  Signdef(const UTF8* name, std::vector<Typedef*>& args, Typedef* ret);
  
//===----------------------------------------------------------------------===//
//
// Inline calls to get the dynamically generated functions to call Java
// functions. Note that this calls the JIT.
//
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
//
// End of inlined methods of getting dynamically generated functions.
//
//===----------------------------------------------------------------------===//
    
  /// nbArguments - The number of arguments in the signature. 
  ///
  uint32 nbArguments;
  
  /// getReturnType - Get the type of the return of this signature.
  ///
  Typedef* getReturnType() const {
    return arguments[0];
  }

  /// getArgumentsType - Get the list of arguments of this signature.
  ///
  Typedef* const* getArgumentsType() const {
    return &(arguments[1]);
  }

  uint32 getNumberOfSlots() {
    uint32 result = nbArguments;
    for (uint32 i = 1; i < nbArguments + 1; i++) {
      if (arguments[i]->isDouble() || arguments[i]->isLong()) {
        result++;
      }
    }
    return result;
  }

private:

  /// arguments - The list of arguments of the signature. First is the return
  /// type.
  ///
  Typedef* arguments[1];

  friend class JavaAOTCompiler;
};

} // end namespace j3

#endif
