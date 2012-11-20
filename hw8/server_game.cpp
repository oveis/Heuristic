#include <cassert>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include "server_game.h"

using std::cout;
using std::endl;
using std::ostringstream;
using std::istringstream;

ServerGame::ServerGame(const char* plyName, Someone* myPly, int nFeatures,
    int numThrs, int srv_port) : Game(plyName, nFeatures, numThrs),
                                 my_ply_(myPly),
                                 noises_(new double[nFeatures]),
                                 person_step_(0),
                                 matcher_step_(0),
                                 arch_clt_(registerSrv(srv_port)) { }

ServerGame::~ServerGame() { }

void ServerGame::startGame() {
  string fromPly = ply_name_, fromSrv, srvEndMark("\n");
  fromPly.push_back('\n');

  try {
    (*arch_clt_) << fromPly;  // Send server our player's name

    do {
      string tmpFromSrv;
      /*
      do {
        (*arch_clt_) >> tmpFromSrv;
        fromSrv += tmpFromSrv;
      } while (fromSrv.find(srvEndMark) == string::npos);
      */
      (*arch_clt_) >> fromSrv;
      cout << "#FromSrv: " << fromSrv << "-----End of #FromSrv" << endl;

      if (!readSrvOutput(fromSrv)) {
        break;
      }
      fromSrv.clear();

      sleep(1);
    } while (1);
  } catch (SocketException& se) {
    assert(false);
  }

}

bool ServerGame::readSrvOutput(const string& fromSrv) {
  int nn;
  string toSrv;

  if (!fromSrv.compare(0, 2, "M ")) {
    nn = atoi(fromSrv.substr(2).c_str());
    cout << "I'm (expect: Matcher): " << ((my_ply_->isMatchmaker())?
      "Matcher":"Person") << " N(expect: " << n_features_ << "): " << nn << endl;
  } else if (!fromSrv.compare(0, 2, "P ")) {
    nn = atoi(fromSrv.substr(2).c_str());
    cout << "I'm (expect: Person): " << ((my_ply_->isMatchmaker())?
      "Matcher":"Person") << " N(expect: " << n_features_ << "): " << nn << endl;
  } else if (!fromSrv.compare(0, 7, "WEIGHTS")) {
    cout << "I'm (expect: Person): " << ((my_ply_->isMatchmaker())?
      "Matcher":"Person") << " Sending Exact Weights." << endl;
    my_ply_->sendOutVector(w_arr_);
    toSrv = printNVectorToSrv(w_arr_);
    cout << "#Person sent to Srv: " << toSrv << endl;
    ++person_step_;
    (*arch_clt_) << toSrv;
  } else if (!fromSrv.compare(0, 5, "NOISE")) {
    cout << "I'm (expect: Person): " << ((my_ply_->isMatchmaker())?
      "Matcher":"Person") << " Sending Noises." << endl;
    my_ply_->sendOutVector(noises_);
    toSrv = printNVectorToSrv(noises_);
    cout << "#Person sent to Srv: " << toSrv << endl;
    ++person_step_;
    (*arch_clt_) << toSrv;
  } else if (!fromSrv.compare(0, 1, "[")) {
    cout << "I'm (expect: Matcher): " << ((my_ply_->isMatchmaker())?
      "Matcher":"Person") << " Receiving 20 srv random candidates." << endl;
    // Receiving 20 random candidates from server, update xx_matr_ and match_score_
    ++matcher_step_;
  } else if (!fromSrv.compare(0, 9, "CANDIDATE")) {
    cout << "I'm (expect: Matcher): " << ((my_ply_->isMatchmaker())?
      "Matcher":"Person") << " Sending Candidate." << endl;
    double tmpArr[n_features_];
    my_ply_->sendOutVector(tmpArr);
    toSrv = printNVectorToSrv(tmpArr);
    cout << "#Matcher sent to Srv: " << toSrv << endl;
    (*arch_clt_) << toSrv;
    ++matcher_step_;
  } else if (!fromSrv.compare(0, 2, "OK") || !fromSrv.compare(0, 5, "ERROR")) {
    cout << "Got msg from Srv: " << fromSrv << endl;
  } else if (!fromSrv.compare(0, 8, "GAMEOVER")) {
    cout << "Got msg from Srv: " << fromSrv << endl;
    return false;
  } else {
    double value;
    istringstream ist(fromSrv);
    ist >> value;
    cout << "Got value from srv: " << value << endl;
  }

  return true;
}

// Array @nVect has to be length of "N"
string ServerGame::printNVectorToSrv(const double* nVect) const {
  ostringstream ost;
  ost << "[";
  for (int i = 0; i < n_features_; ++i) {
    if (i != (n_features_ - 1)) {
      ost << nVect[i] << ", ";
    } else {
      ost << nVect[i] << "]" << endl;  // Terminate string by '\n'
    }
  }

  return ost.str();
}

ClientSocket* ServerGame::registerSrv(int srv_port) {
  ClientSocket* pClt_socket = NULL;

  try {
    pClt_socket = new ClientSocket("localhost", srv_port);
  } catch (SocketException& e) {
    std::cout << "Exception was caught:" << e.description() << " PortNum: "
      << srv_port << std::endl;
    assert(0);
  }

  return pClt_socket;
}
