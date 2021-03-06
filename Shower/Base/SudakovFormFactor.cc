// -*- C++ -*-
//
// SudakovFormFactor.cc is a part of Herwig - A multi-purpose Monte Carlo event generator
// Copyright (C) 2002-2011 The Herwig Collaboration
//
// Herwig is licenced under version 2 of the GPL, see COPYING for details.
// Please respect the MCnet academic guidelines, see GUIDELINES for details.
//
//
// This is the implementation of the non-inlined, non-templated member
// functions of the SudakovFormFactor class.
//

#include "SudakovFormFactor.h"
#include "ThePEG/Interface/ClassDocumentation.h"
#include "ThePEG/Persistency/PersistentOStream.h"
#include "ThePEG/Persistency/PersistentIStream.h"
#include "ThePEG/Interface/Reference.h"
#include "ThePEG/Interface/Switch.h"
#include "ThePEG/Interface/Parameter.h"
#include "ShowerKinematics.h"
#include "ShowerParticle.h"
#include "ThePEG/Helicity/WaveFunction/SpinorWaveFunction.h"
#include "ThePEG/Helicity/WaveFunction/SpinorBarWaveFunction.h"
#include "ThePEG/Helicity/WaveFunction/SpinorWaveFunction.h"
#include "ThePEG/Helicity/WaveFunction/VectorWaveFunction.h"
#include "ThePEG/Helicity/WaveFunction/ScalarWaveFunction.h"
#include "ThePEG/Utilities/DescribeClass.h"
#include "Herwig/Shower/ShowerHandler.h"

using namespace Herwig;

DescribeAbstractClass<SudakovFormFactor,Interfaced>
describeSudakovFormFactor ("Herwig::SudakovFormFactor","");

void SudakovFormFactor::persistentOutput(PersistentOStream & os) const {
  os << splittingFn_ << alpha_ << pdfmax_ << particles_ << pdffactor_
     << a_ << b_ << ounit(c_,GeV) << ounit(kinCutoffScale_,GeV) << cutOffOption_
     << ounit(vgcut_,GeV) << ounit(vqcut_,GeV) 
     << ounit(pTmin_,GeV) << ounit(pT2min_,GeV2)
     << theFactorizationScaleFactor << theRenormalizationScaleFactor;
}

void SudakovFormFactor::persistentInput(PersistentIStream & is, int) {
  is >> splittingFn_ >> alpha_ >> pdfmax_ >> particles_ >> pdffactor_
     >> a_ >> b_ >> iunit(c_,GeV) >> iunit(kinCutoffScale_,GeV) >> cutOffOption_
     >> iunit(vgcut_,GeV) >> iunit(vqcut_,GeV) 
     >> iunit(pTmin_,GeV) >> iunit(pT2min_,GeV2)
     >> theFactorizationScaleFactor >> theRenormalizationScaleFactor;
}

void SudakovFormFactor::Init() {

  static ClassDocumentation<SudakovFormFactor> documentation
    ("The SudakovFormFactor class is the base class for the implementation of Sudakov"
     " form factors in Herwig");

  static Reference<SudakovFormFactor,SplittingFunction>
    interfaceSplittingFunction("SplittingFunction",
			       "A reference to the SplittingFunction object",
			       &Herwig::SudakovFormFactor::splittingFn_,
			       false, false, true, false);

  static Reference<SudakovFormFactor,ShowerAlpha>
    interfaceAlpha("Alpha",
		   "A reference to the Alpha object",
		   &Herwig::SudakovFormFactor::alpha_,
		   false, false, true, false);

  static Parameter<SudakovFormFactor,double> interfacePDFmax
    ("PDFmax",
     "Maximum value of PDF weight. ",
     &SudakovFormFactor::pdfmax_, 35.0, 1.0, 100000.0,
     false, false, Interface::limited);

  static Switch<SudakovFormFactor,unsigned int> interfacePDFFactor
    ("PDFFactor",
     "Include additional factors in the overestimate for the PDFs",
     &SudakovFormFactor::pdffactor_, 0, false, false);
  static SwitchOption interfacePDFFactorOff
    (interfacePDFFactor,
     "Off",
     "Don't include any factors",
     0);
  static SwitchOption interfacePDFFactorOverZ
    (interfacePDFFactor,
     "OverZ",
     "Include an additional factor of 1/z",
     1);
  static SwitchOption interfacePDFFactorOverOneMinusZ
    (interfacePDFFactor,
     "OverOneMinusZ",
     "Include an additional factor of 1/(1-z)",
     2);
  static SwitchOption interfacePDFFactorOverZOneMinusZ
    (interfacePDFFactor,
     "OverZOneMinusZ",
     "Include an additional factor of 1/z/(1-z)",
     3);


  static Switch<SudakovFormFactor,unsigned int> interfaceCutOffOption
    ("CutOffOption",
     "The type of cut-off to use to end the shower",
     &SudakovFormFactor::cutOffOption_, 0, false, false);
  static SwitchOption interfaceCutOffOptionDefault
    (interfaceCutOffOption,
     "Default",
     "Use the standard Herwig cut-off on virtualities with the minimum"
     " virtuality depending on the mass of the branching particle",
     0);
  static SwitchOption interfaceCutOffOptionFORTRAN
    (interfaceCutOffOption,
     "FORTRAN",
     "Use a FORTRAN-like cut-off on virtualities",
     1);
  static SwitchOption interfaceCutOffOptionpT
    (interfaceCutOffOption,
     "pT",
     "Use a cut on the minimum allowed pT",
     2);

  static Parameter<SudakovFormFactor,double> interfaceaParameter
    ("aParameter",
     "The a parameter for the kinematic cut-off",
     &SudakovFormFactor::a_, 0.3, -10.0, 10.0,
     false, false, Interface::limited);

  static Parameter<SudakovFormFactor,double> interfacebParameter
    ("bParameter",
     "The b parameter for the kinematic cut-off",
     &SudakovFormFactor::b_, 2.3, -10.0, 10.0,
     false, false, Interface::limited);

  static Parameter<SudakovFormFactor,Energy> interfacecParameter
    ("cParameter",
     "The c parameter for the kinematic cut-off",
     &SudakovFormFactor::c_, GeV, 0.3*GeV, 0.1*GeV, 10.0*GeV,
     false, false, Interface::limited);

  static Parameter<SudakovFormFactor,Energy>
    interfaceKinScale ("cutoffKinScale",
		       "kinematic cutoff scale for the parton shower phase"
		       " space (unit [GeV])",
		       &SudakovFormFactor::kinCutoffScale_, GeV, 
		       2.3*GeV, 0.001*GeV, 10.0*GeV,false,false,false);
  
  static Parameter<SudakovFormFactor,Energy> interfaceGluonVirtualityCut
    ("GluonVirtualityCut",
     "For the FORTRAN cut-off option the minimum virtuality of the gluon",
     &SudakovFormFactor::vgcut_, GeV, 0.85*GeV, 0.1*GeV, 10.0*GeV,
     false, false, Interface::limited);
  
  static Parameter<SudakovFormFactor,Energy> interfaceQuarkVirtualityCut
    ("QuarkVirtualityCut",
     "For the FORTRAN cut-off option the minimum virtuality added to"
     " the mass for particles other than the gluon",
     &SudakovFormFactor::vqcut_, GeV, 0.85*GeV, 0.1*GeV, 10.0*GeV,
     false, false, Interface::limited);
  
  static Parameter<SudakovFormFactor,Energy> interfacepTmin
    ("pTmin",
     "The minimum pT if using a cut-off on the pT",
     &SudakovFormFactor::pTmin_, GeV, 1.0*GeV, ZERO, 10.0*GeV,
     false, false, Interface::limited);
}

bool SudakovFormFactor::alphaSVeto(Energy2 pt2) const {
  double ratio=alphaSVetoRatio(pt2,1.);
  return UseRandom::rnd() > ratio;
}

double SudakovFormFactor::alphaSVetoRatio(Energy2 pt2, double factor) const {
  factor *= renormalizationScaleFactor();
  return  ThePEG::Math::powi(alpha_->ratio(pt2, factor),
                               splittingFn_->interactionOrder());
}


bool SudakovFormFactor::PDFVeto(const Energy2 t, const double x,
	const tcPDPtr parton0, const tcPDPtr parton1,
	Ptr<BeamParticleData>::transient_const_pointer beam) const {
  double ratio=PDFVetoRatio(t,x,parton0,parton1,beam,1.);
  return UseRandom::rnd() > ratio;
}

double SudakovFormFactor::PDFVetoRatio(const Energy2 t, const double x,
        const tcPDPtr parton0, const tcPDPtr parton1,
        Ptr<BeamParticleData>::transient_const_pointer beam,double factor) const {
  assert(pdf_);

  Energy2 theScale = t * sqr(factorizationScaleFactor()*factor);
  if (theScale < sqr(freeze_)) theScale = sqr(freeze_);

  double newpdf(0.0), oldpdf(0.0);

  newpdf=pdf_->xfx(beam,parton0,theScale,x/z());
  oldpdf=pdf_->xfx(beam,parton1,theScale,x);

  if(newpdf<=0.) return 0.;
  if(oldpdf<=0.) return 1.;
  
  double ratio = newpdf/oldpdf;
  double maxpdf = pdfmax_;

  switch (pdffactor_) {
  case 1:
    maxpdf /= z();
    break;
  case 2:
    maxpdf /= 1.-z();
    break;
  case 3:
    maxpdf /= (z()*(1.-z()));
    break;
  }

  if (ratio > maxpdf) {
    generator()->log() << "PDFVeto warning: Ratio > " << name()
                       << ":PDFmax (by a factor of "
                       << ratio/maxpdf <<") for "
                       << parton0->PDGName() << " to "
                       << parton1->PDGName() << "\n";
  }
  return ratio/maxpdf ;
}








void SudakovFormFactor::addSplitting(const IdList & in) {
  bool add=true;
  for(unsigned int ix=0;ix<particles_.size();++ix) {
    if(particles_[ix].size()==in.size()) {
      bool match=true;
      for(unsigned int iy=0;iy<in.size();++iy) {
	if(particles_[ix][iy]!=in[iy]) {
	  match=false;
	  break;
	}
      }
      if(match) {
	add=false;
	break;
      }
    }
  }
  if(add) particles_.push_back(in);
}

namespace {

LorentzRotation boostToShower(const vector<Lorentz5Momentum> & basis,
			      ShowerKinematics::Frame frame,
			      Lorentz5Momentum & porig) {
  LorentzRotation output;
  if(frame==ShowerKinematics::BackToBack) {
    // we are doing the evolution in the back-to-back frame
    // work out the boostvector
    Boost boostv(-(basis[0]+basis[1]).boostVector());
    // momentum of the parton
    Lorentz5Momentum ptest(basis[0]);
    // construct the Lorentz boost
    output = LorentzRotation(boostv);
    ptest *= output;
    Axis axis(ptest.vect().unit());
    // now rotate so along the z axis as needed for the splitting functions
    if(axis.perp2()>1e-10) {
      double sinth(sqrt(1.-sqr(axis.z())));
      output.rotate(-acos(axis.z()),Axis(-axis.y()/sinth,axis.x()/sinth,0.));
    }
    else if(axis.z()<0.) {
      output.rotate(Constants::pi,Axis(1.,0.,0.));
    }
    porig = output*basis[0];
    porig.setX(ZERO);
    porig.setY(ZERO);
  }
  else {
    output = LorentzRotation(-basis[0].boostVector());
    porig = output*basis[0];
    porig.setX(ZERO);
    porig.setY(ZERO);
    porig.setZ(ZERO);
  }
  return output;
}

RhoDMatrix bosonMapping(ShowerParticle & particle,
			const Lorentz5Momentum & porig,
			VectorSpinPtr vspin,
			const LorentzRotation & rot) {
  // rotate the original basis
  vector<LorentzPolarizationVector> sbasis;
  for(unsigned int ix=0;ix<3;++ix) {
    sbasis.push_back(vspin->getProductionBasisState(ix));
    sbasis.back().transform(rot);
  }
  // splitting basis
  vector<LorentzPolarizationVector> fbasis;
  bool massless(particle.id()==ParticleID::g||particle.id()==ParticleID::gamma);
  VectorWaveFunction wave(porig,particle.dataPtr(),outgoing);
  for(unsigned int ix=0;ix<3;++ix) {
    if(massless&&ix==1) {
      fbasis.push_back(LorentzPolarizationVector());
    }
    else {
      wave.reset(ix);
      fbasis.push_back(wave.wave());
    }
  }
  // work out the mapping
  RhoDMatrix mapping=RhoDMatrix(PDT::Spin1,false);
  for(unsigned int ix=0;ix<3;++ix) {
    for(unsigned int iy=0;iy<3;++iy) {
      mapping(ix,iy)= sbasis[iy].dot(fbasis[ix].conjugate());
      if(particle.id()<0)
	mapping(ix,iy)=conj(mapping(ix,iy));
    }
  }
  // \todo need to fix this
  mapping = RhoDMatrix(PDT::Spin1,false);
  if(massless) {
    mapping(0,0) = 1.;
    mapping(2,2) = 1.;
  }
  else {
    mapping(0,0) = 1.;
    mapping(1,1) = 1.;
    mapping(2,2) = 1.;
  }
  return mapping;
}

RhoDMatrix fermionMapping(ShowerParticle & particle,
			  const Lorentz5Momentum & porig,
			  FermionSpinPtr fspin,
			  const LorentzRotation & rot) {
  // extract the original basis states
  vector<LorentzSpinor<SqrtEnergy> > sbasis;
  for(unsigned int ix=0;ix<2;++ix) {
    sbasis.push_back(fspin->getProductionBasisState(ix));
    sbasis.back().transform(rot);
  }
  // calculate the states in the splitting basis
  vector<LorentzSpinor<SqrtEnergy> > fbasis;
  SpinorWaveFunction wave(porig,particle.dataPtr(),
			  particle.id()>0 ? incoming : outgoing);
  for(unsigned int ix=0;ix<2;++ix) {
    wave.reset(ix);
    fbasis.push_back(wave.dimensionedWave());
  }
  RhoDMatrix mapping=RhoDMatrix(PDT::Spin1Half,false);
  for(unsigned int ix=0;ix<2;++ix) {
    if(fbasis[0].s2()==complex<SqrtEnergy>()) {
      mapping(ix,0) = sbasis[ix].s3()/fbasis[0].s3();
      mapping(ix,1) = sbasis[ix].s2()/fbasis[1].s2();
    }
    else {
      mapping(ix,0) = sbasis[ix].s2()/fbasis[0].s2();
      mapping(ix,1) = sbasis[ix].s3()/fbasis[1].s3();
    }
  }
  return mapping;
}

FermionSpinPtr createFermionSpinInfo(ShowerParticle & particle,
				     const Lorentz5Momentum & porig,
				     const LorentzRotation & rot,
				     Helicity::Direction dir) {
  // calculate the splitting basis for the branching
  // and rotate back to construct the basis states
  LorentzRotation rinv = rot.inverse();
  SpinorWaveFunction wave;
  if(particle.id()>0)
    wave=SpinorWaveFunction(porig,particle.dataPtr(),incoming);
  else
    wave=SpinorWaveFunction(porig,particle.dataPtr(),outgoing);
  FermionSpinPtr fspin = new_ptr(FermionSpinInfo(particle.momentum(),dir==outgoing));
  for(unsigned int ix=0;ix<2;++ix) {
    wave.reset(ix);
    LorentzSpinor<SqrtEnergy> basis = wave.dimensionedWave();
    basis.transform(rinv);
    fspin->setBasisState(ix,basis);
    fspin->setDecayState(ix,basis);
  }
  particle.spinInfo(fspin);
  return fspin;
}

VectorSpinPtr createVectorSpinInfo(ShowerParticle & particle,
				   const Lorentz5Momentum & porig,
				   const LorentzRotation & rot,
				   Helicity::Direction dir) {
  // calculate the splitting basis for the branching
  // and rotate back to construct the basis states
  LorentzRotation rinv = rot.inverse();
  bool massless(particle.id()==ParticleID::g||particle.id()==ParticleID::gamma);
  VectorWaveFunction wave(porig,particle.dataPtr(),dir);
  VectorSpinPtr vspin = new_ptr(VectorSpinInfo(particle.momentum(),dir==outgoing));
  for(unsigned int ix=0;ix<3;++ix) {
    LorentzPolarizationVector basis;
    if(massless&&ix==1) {
      basis = LorentzPolarizationVector();
    }
    else {
      wave.reset(ix);
      basis = wave.wave();
    }
    basis *= rinv;
    vspin->setBasisState(ix,basis);
    vspin->setDecayState(ix,basis);
  }
  particle.spinInfo(vspin);
  vspin->  DMatrix() = RhoDMatrix(PDT::Spin1);
  vspin->rhoMatrix() = RhoDMatrix(PDT::Spin1);
  if(massless) {
    vspin->  DMatrix()(0,0) = 0.5;
    vspin->rhoMatrix()(0,0) = 0.5;
    vspin->  DMatrix()(2,2) = 0.5;
    vspin->rhoMatrix()(2,2) = 0.5;
  }
  return vspin;
}
}

bool SudakovFormFactor::getMapping(SpinPtr & output, RhoDMatrix & mapping,
				   ShowerParticle & particle,ShoKinPtr showerkin) {
  // if the particle is not from the hard process
  if(!particle.perturbative()) {
    // mapping is the identity
    output=particle.spinInfo();
    mapping=RhoDMatrix(particle.dataPtr()->iSpin());
    if(output) {
      return false;
    }
    else {
      Lorentz5Momentum porig;
      LorentzRotation rot = boostToShower(showerkin->getBasis(),showerkin->frame(),porig);
      Helicity::Direction dir = particle.isFinalState() ? outgoing : incoming;
      if(particle.dataPtr()->iSpin()==PDT::Spin0) {
	assert(false);
      }
      else if(particle.dataPtr()->iSpin()==PDT::Spin1Half) {
	output = createFermionSpinInfo(particle,porig,rot,dir);
      }
      else if(particle.dataPtr()->iSpin()==PDT::Spin1) {
	output = createVectorSpinInfo(particle,porig,rot,dir);
      }
      else {
	assert(false);
      }
      return false;
    }
  }
  // if particle is final-state and is from the hard process
  else if(particle.isFinalState()) {
    assert(particle.perturbative()==1 || particle.perturbative()==2);
    // get transform to shower frame
    Lorentz5Momentum porig;
    LorentzRotation rot = boostToShower(showerkin->getBasis(),showerkin->frame(),porig);
    // the rest depends on the spin of the particle
    PDT::Spin spin(particle.dataPtr()->iSpin());
    mapping=RhoDMatrix(spin,false);
    // do the spin dependent bit
    if(spin==PDT::Spin0) {
      ScalarSpinPtr sspin=dynamic_ptr_cast<ScalarSpinPtr>(particle.spinInfo());
      if(!sspin) {
	ScalarWaveFunction::constructSpinInfo(&particle,outgoing,true);
      }
      output=particle.spinInfo();
      return false;
    }
    else if(spin==PDT::Spin1Half) {
      FermionSpinPtr fspin=dynamic_ptr_cast<FermionSpinPtr>(particle.spinInfo());
      // spin info exists get information from it
      if(fspin) {
	output=fspin;
	mapping = fermionMapping(particle,porig,fspin,rot);
	return true;
      }
      // spin info does not exist create it
      else {
	output = createFermionSpinInfo(particle,porig,rot,outgoing);
	return false;
      }
    }
    else if(spin==PDT::Spin1) {
      VectorSpinPtr vspin=dynamic_ptr_cast<VectorSpinPtr>(particle.spinInfo());
      // spin info exists get information from it
      if(vspin) {
	output=vspin;
	mapping = bosonMapping(particle,porig,vspin,rot);
	return true; 
      }
      else {
	output = createVectorSpinInfo(particle,porig,rot,outgoing);
	return false;
      }
    }
    // not scalar/fermion/vector
    else
      assert(false);
  }
  // incoming to hard process
  else if(particle.perturbative()==1 && !particle.isFinalState()) {
    // get the basis vectors
    // get transform to shower frame
    Lorentz5Momentum porig;
    LorentzRotation rot = boostToShower(showerkin->getBasis(),showerkin->frame(),porig);
    porig *= particle.x();
    // the rest depends on the spin of the particle
    PDT::Spin spin(particle.dataPtr()->iSpin());
    mapping=RhoDMatrix(spin);
    // do the spin dependent bit
    if(spin==PDT::Spin0) {
      cerr << "testing spin 0 not yet implemented " << endl;
      assert(false);
    }
    // spin-1/2
    else if(spin==PDT::Spin1Half) {
      FermionSpinPtr fspin=dynamic_ptr_cast<FermionSpinPtr>(particle.spinInfo());
      // spin info exists get information from it
      if(fspin) {
	output=fspin;
	mapping = fermionMapping(particle,porig,fspin,rot);
	return true;
      }
      // spin info does not exist create it
      else {
	output = createFermionSpinInfo(particle,porig,rot,incoming);
	return false;
      }
    }
    // spin-1
    else if(spin==PDT::Spin1) {
      VectorSpinPtr vspin=dynamic_ptr_cast<VectorSpinPtr>(particle.spinInfo());
      // spinInfo exists map it
      if(vspin) {
	output=vspin;
	mapping = bosonMapping(particle,porig,vspin,rot);
	return true;
      }
      // create the spininfo
      else {
	output = createVectorSpinInfo(particle,porig,rot,incoming);
	return false;
      }
    }
    assert(false);
  }
  // incoming to decay
  else if(particle.perturbative() == 2 && !particle.isFinalState()) { 
    // get the basis vectors
    Lorentz5Momentum porig;
    LorentzRotation rot=boostToShower(showerkin->getBasis(),
				      showerkin->frame(),porig);
    // the rest depends on the spin of the particle
    PDT::Spin spin(particle.dataPtr()->iSpin());
    mapping=RhoDMatrix(spin);
    // do the spin dependent bit
    if(spin==PDT::Spin0) {
      cerr << "testing spin 0 not yet implemented " << endl;
      assert(false);
    }
    // spin-1/2
    else if(spin==PDT::Spin1Half) {
    //   FermionSpinPtr fspin=dynamic_ptr_cast<FermionSpinPtr>(particle.spinInfo());
    //   // spin info exists get information from it
    //   if(fspin) {
    // 	output=fspin;
    // 	mapping = fermionMapping(particle,porig,fspin,rot);
    // 	return true;
    //   // spin info does not exist create it
    //   else {
    // 	output = createFermionSpinInfo(particle,porig,rot,incoming);
    // 	return false;
    //   }
    // }
      assert(false);
    }
    // // spin-1
    // else if(spin==PDT::Spin1) {
    //   VectorSpinPtr vspin=dynamic_ptr_cast<VectorSpinPtr>(particle.spinInfo());
    //   // spinInfo exists map it
    //   if(vspin) {
    // 	output=vspin;
    // 	mapping = bosonMapping(particle,porig,vspin,rot);
    // 	return true;
    //   }
    //   // create the spininfo
    //   else {
    // 	output = createVectorSpinInfo(particle,porig,rot,incoming);
    // 	return false;
    //   }
    // }
    // assert(false);
    assert(false);
  }
  else
    assert(false);
  return true;
}

void SudakovFormFactor::removeSplitting(const IdList & in) {
  for(vector<IdList>::iterator it=particles_.begin();
      it!=particles_.end();++it) {
    if(it->size()==in.size()) {
      bool match=true;
      for(unsigned int iy=0;iy<in.size();++iy) {
	if((*it)[iy]!=in[iy]) {
	  match=false;
	  break;
	}
      }
      if(match) {
	vector<IdList>::iterator itemp=it;
	--itemp;
	particles_.erase(it);
	it = itemp;
      }
    }
  }
}

Energy2 SudakovFormFactor::guesst(Energy2 t1,unsigned int iopt,
				  const IdList &ids,
				  double enhance,bool ident,
				  const double & detune) const {
  unsigned int pdfopt = iopt!=1 ? 0 : pdffactor_;
  double c =
    1./((splittingFn_->integOverP(zlimits_.second,ids,pdfopt) -
	 splittingFn_->integOverP(zlimits_.first ,ids,pdfopt))* 
	alpha_->overestimateValue()/Constants::twopi*enhance*detune);
  assert(iopt<=2);
  if(iopt==1) {
    c/=pdfmax_;
    //symmetry of FS gluon splitting
    if(ident) c*=0.5;
  }
  else if(iopt==2) c*=-1.;
  if(splittingFn_->interactionOrder()==1) {
    double r = UseRandom::rnd();
    if(iopt!=2 || c*log(r)<log(Constants::MaxEnergy2/t1)) {
      return t1*pow(r,c);
    }
    else
      return Constants::MaxEnergy2;
  }
  else {
    assert(false && "Units are dubious here.");
    int nm(splittingFn()->interactionOrder()-1);
    c/=Math::powi(alpha_->overestimateValue()/Constants::twopi,nm);
    return t1 /       pow (1. - nm*c*log(UseRandom::rnd()) 
			   * Math::powi(t1*UnitRemoval::InvE2,nm)
			   ,1./double(nm));
  }
}

double SudakovFormFactor::guessz (unsigned int iopt, const IdList &ids) const {
  unsigned int pdfopt = iopt!=1 ? 0 : pdffactor_;
  double lower = splittingFn_->integOverP(zlimits_.first,ids,pdfopt);
  return splittingFn_->invIntegOverP
    (lower + UseRandom::rnd()*(splittingFn_->integOverP(zlimits_.second,ids,pdfopt) - 
			       lower),ids,pdfopt);
}

void SudakovFormFactor::doinit() {
  Interfaced::doinit();
  pT2min_ = cutOffOption()==2 ? sqr(pTmin_) : ZERO; 
}

const vector<Energy> & SudakovFormFactor::virtualMasses(const IdList & ids) {
  static vector<Energy> output;
  output.clear();
  if(cutOffOption() == 0) {
    for(unsigned int ix=0;ix<ids.size();++ix)
      output.push_back(getParticleData(ids[ix])->mass());
    Energy kinCutoff=
      kinematicCutOff(kinScale(),*std::max_element(output.begin(),output.end()));
    for(unsigned int ix=0;ix<output.size();++ix)
      output[ix]=max(kinCutoff,output[ix]);
  }
  else if(cutOffOption() == 1) {
    for(unsigned int ix=0;ix<ids.size();++ix) {
      output.push_back(getParticleData(ids[ix])->mass());
      output.back() += ids[ix]==ParticleID::g ? vgCut() : vqCut();
    }
  }
  else if(cutOffOption() == 2) {
    for(unsigned int ix=0;ix<ids.size();++ix) 
      output.push_back(getParticleData(ids[ix])->mass());
  }
  else {
    throw Exception() << "Unknown option for the cut-off"
		      << " in SudakovFormFactor::virtualMasses()"
		      << Exception::runerror;
  }
  return output;
}
