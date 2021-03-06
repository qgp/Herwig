// -*- C++ -*-
//
// GenericSVVVertex.h is a part of Herwig - A multi-purpose Monte Carlo event generator
// Copyright (C) 2002-2011 The Herwig Collaboration
//
// Herwig is licenced under version 2 of the GPL, see COPYING for details.
// Please respect the MCnet academic guidelines, see GUIDELINES for details.
//
#ifndef HERWIG_GenericSVVVertex_H
#define HERWIG_GenericSVVVertex_H
//
// This is the declaration of the GenericSVVVertex class.
//

#include "ThePEG/Helicity/Vertex/Scalar/GeneralVVSVertex.h"



namespace Herwig {
using namespace ThePEG;

/**
 * The <code>GenericSVVVertex</code> class implements the 
 * setCoupling member for the Standard Model effective 
 * vertex Higgs-gluon-gluon. 
 */
class GenericSVVVertex: public Helicity::GeneralVVSVertex {
  
public:

  /**
   * The default constructor.
   */
  GenericSVVVertex();

  /** @name Functions used by the persistent I/O system. */
  //@{
  /**
   * Function used to write out object persistently.
   * @param os the persistent output stream written to.
   */
  void persistentOutput(PersistentOStream & os) const;

  /**
   * Function used to read in object persistently.
   * @param is the persistent input stream read from.
   * @param version the version number of the object when written.
   */
  void persistentInput(PersistentIStream & is, int version);
  //@}

  /**
   * The standard Init function used to initialize the interfaces.
   * Called exactly once for each class by the class description system
   * before the main function starts or
   * when this class is dynamically loaded.
   */
  static void Init();

  /** 
   * Calculate couplings
   *@param q2 Scale at which to evaluate coupling
   *@param part1 ParticleData pointer to first particle
   *@param part2 ParticleData pointer to first particle
   *@param part3 ParticleData pointer to first particle
   */
  virtual void setCoupling (Energy2 q2, tcPDPtr part1, tcPDPtr part2, tcPDPtr part3);

protected:

  /** @name Clone Methods. */
  //@{
  /**
   * Make a simple clone of this object.
   * @return a pointer to the new object.
   */
  virtual IBPtr clone() const {return new_ptr(*this);}

  /** Make a clone of this object, possibly modifying the cloned object
   * to make it sane.
   * @return a pointer to the new object.
   */
  virtual IBPtr fullclone() const {return new_ptr(*this);}
  //@}

  /**
   * Initialize this object after the setup phase before saving and
   * EventGenerator to disk.
   * @throws InitException if object could not be initialized properly.
   */
  virtual void doinit();
  
  
  string dopids(string in);

private:
  /**
   * The static object used to initialize the description of this class.
   * Indicates that this is a concrete class with persistent data.
   */
  static ClassDescription<GenericSVVVertex> initGenericSVVVertex;

  /**
   * The assignment operator is private and must never be called.
   * In fact, it should not even be implemented.
   */
  GenericSVVVertex & operator=(const GenericSVVVertex &);

  /**
   * Storage of couplings
   */
  //@{

  
  /**
   * The particle ids 
   */
  vector <int> pids;

  /**
   * The particle ids 
   */
  int oas,oaew;


};

}

// CLASSDOC OFF

#include "ThePEG/Utilities/ClassTraits.h"

namespace ThePEG {

/** @cond TRAITSPECIALIZATIONS */

/** This template specialization informs ThePEG about the
 *  base classes of GenericSVVVertex. */
template <>
struct BaseClassTrait<Herwig::GenericSVVVertex,1> {
  /** Typedef of the first base class of GenericSVVVertex. */
  typedef Helicity::GeneralVVSVertex  NthBase;
};

/** This template specialization informs ThePEG about the name of
 *  the GenericSVVVertex class and the shared object where it is defined. */
template <>
struct ClassTraits<Herwig::GenericSVVVertex>
  : public ClassTraitsBase<Herwig::GenericSVVVertex> {
  /** Return a platform-independent class name */
  static string className() { return "Herwig::GenericSVVVertex"; }
};

/** @endcond */

}

#endif /* HERWIG_GenericSVVVertex_H */
