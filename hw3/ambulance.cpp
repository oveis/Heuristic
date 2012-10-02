#include "ambulance.h" 
#include "patient.h"
#include <iostream>
using namespace std;
Ambulance::Ambulance(Hospital* h,int i,vector<Patient*> *p, vector<Hospital*> *hos):
  id(i),
  next_available_time_(0),
  patients(p),
  hospitals(hos),
  saved_count_(0){
  routine.push_back(h);
}
void Ambulance::output() {
  int time = 0;
  for (int i = 0; i< routine.size();i++) {
    routine[i]->output();
    if (i!=routine.size()-1)
      cout<<",";
    else
      cout<< "\n";
    if (i+1<routine.size())
      time+= routine[i]->distance(routine[i+1])+1;
  }
}
bool Ambulance::addRountine(Position* p){
  routine.push_back(p);
  return true;
}
void Ambulance::move2() {
  Patient* nextToSave;
  while (couldSaveMore() || onboard_.size()>0) {
    if (onboard_.size()<4) {
      nextToSave = findNextToSave2();
      if (nextToSave!=NULL)
        goToSave(nextToSave);
      else if (onboard_.size()>0)
        goToNearestHospital();
      else return;
    }
    else // it's full now
      goToNearestHospital ();
  }
}
void Ambulance::move() {
  Patient* nextToSave;
  while (couldSaveMore() || onboard_.size()>0) {
    if (onboard_.size()<4) {
      nextToSave = findNextToSave();
      if (nextToSave!=NULL)
        goToSave(nextToSave);
      else if (onboard_.size()>0)
        goToNearestHospital();
      else return;
    }
    else // it's full now
      goToNearestHospital ();
  }
}
void Ambulance::goToNearestHospital () {
  Hospital*h =((Patient*) routine[routine.size()-1])->getNearestHospital();
  int time = routine[routine.size()-1]->distance(h)+1;
  routine.push_back(h);
  next_available_time_+=time;
  onboard_.clear();
}
void Ambulance::goToSave(Patient*p) {
  int time = routine[routine.size()-1]->distance(p)+1;
  routine.push_back(p);
  onboard_.push_back(p);
  p->setSaved();
  next_available_time_+=time;
  saved_count_++;
}
bool Ambulance::couldSave(Patient*p){
  int goThereTime = routine[routine.size()-1]->distance(p)+1;
  int thenToHosTime = p->distance(p->getNearestHospital())+1;
  int finalTime =
    next_available_time_+
    goThereTime+
    thenToHosTime;

  if (finalTime>p->getRescueTime())
    return false;
  for (int i = 0; i<onboard_.size();i++)
    if (finalTime>onboard_[i]->getRescueTime())
      return false;
  return true;
}

bool Ambulance::couldSaveMore() {
  for (int i = 0; i<patients->size();i++)
    if (patients->at(i)->getSaved()==false && couldSave(patients->at(i)))
      return true;
  return false;
}

Patient* Ambulance::findNextToSave() {
  Patient * ret = NULL;
  Patient * tmp;
  Position* currentPos = routine[routine.size()-1];
  for (int i = 0; i<patients->size();i++) {
    tmp = patients->at(i);
    if (tmp->getSaved()==false && couldSave(tmp))
    // find better heuristic to decide this
      if (ret == NULL || ret->distance(currentPos)>tmp->distance(currentPos))
        ret = tmp;
  }
  return ret;
}
Patient* Ambulance::findNextToSave2() {
  Patient * ret = NULL;
  Patient * tmp;
  vector<Patient*> ps;
  vector<Patient*> ps2;
  Position* currentPos = routine[routine.size()-1];
  for (int i = 0; i<patients->size();i++) {
    tmp = patients->at(i);
    if (tmp->getSaved()==false && couldSave(tmp))
      ps.push_back(tmp);
  }
  if (ps.size() == 0) return NULL;
  int ind;
  if (rand()%5 != 0) {
    for (int i = 0; i<ps.size();i++)
      for (int j = i+1;j<ps.size();j++)
	if (ps[i]->distance(currentPos)>
	    ps[j]->distance(currentPos)) {
	  Patient * p = ps[i];
	  ps[i] = ps[j];
	  ps[j] = p;
	}
    ind = rand()%3%ps.size();
  }
  else {
    for (int i = 0; i<ps.size();i++)
      for (int j = i+1;j<ps.size();j++)
	if (ps[i]->getRescueTime() - ps[i]->distance(currentPos) - ps[i]->disToNH() - next_available_time_ >
	    ps[j]->getRescueTime() - ps[j]->distance(currentPos) - ps[i]->disToNH() - next_available_time_) {
	  Patient * p = ps[i];
	  ps[i] = ps[j];
	  ps[j] = p;
	}
    ind = rand()%3%ps.size();
  }
  //  cout<<ind<<endl;
  return ps[ind];
}
