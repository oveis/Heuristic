#include <cstring>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <sstream>
#include "evasion.h"

using std::stringstream;
using std::setw;

Evasion::Evasion(Moveable* moveable, int n, int m, int srv_port)
    : N_(n), M_(m), my_obj_(moveable), arch_clt_(registerSrv(srv_port)) {
  my_obj_->setEvadeGamePtr(this);
  // Insert game borders as special walls (No thickness)
  hor_walls_.push_back(new Moveable::Wall(0, 0, 500, 0, 0));
  hor_walls_.push_back(new Moveable::Wall(0, 500, 500, 500, 2));
  ver_walls_.push_back(new Moveable::Wall(0, 0, 0, 500, 1));
  ver_walls_.push_back(new Moveable::Wall(500, 0, 500, 500, 3));
  // matrix graph showing adjacency between walls
  adj_walls_ = new int*[M_ + 4];
  for (int i = 0; i < M_ + 4; ++i) {
    adj_walls_[i] = new int[M_ + 4];
    memset(adj_walls_[i], 0, sizeof(int) * (M_ + 4));
  }
  // Set init adjacency between game borders
  for (int i = 1; i < 4; ++i) {
    adj_walls_[i - 1][i] = 1;
    adj_walls_[i][i - 1] = 1;
    adj_walls_[i][(i + 1) % 4] = 1;
    adj_walls_[(i + 1) % 4][i] = 1;
  }
  wall_index = 4;
  n_count = 0;
  m_count = 0;
  h_pos.set(0, 0);
  p_pos.set(330, 200);
}

Evasion::~Evasion() {
  delete my_obj_;
  delete arch_clt_;
  hor_walls_.clear();
  ver_walls_.clear();
  for (int i = 0; i < M_ + 4; ++i)
    delete [] adj_walls_[i];
  delete [] adj_walls_;
}

int Evasion::startGame(string& teamName) {
  int gameResult = playing;
  string fromSrv, fromPly;
  teamName.push_back('\n');
  

  try {
    (*arch_clt_) << teamName;

    do {
      (*arch_clt_) >> fromSrv;
      if (fromSrv.empty() || fromSrv.compare("Bye") == 0)
        break;

      n_count++;
      // Prey Mode
      if(!my_obj_->isHunter()){ 
	Moveable::HuntPreyOutput output = my_obj_->tryMove();
      }
      // Hunter mode
      else if(my_obj_->isHunter() && n_count >= N_){  
	Moveable::HuntPreyOutput output = my_obj_->tryMove();
	if(output.x1 != -1 && output.y1 != -1 && output.x2 != -1 && output.y2 != -1){
	  stringstream ss;
	  ss << output.x1 << " " << output.y1 << " " << output.x2 << " " << output.y2;
	  (*arch_clt_) << ss.str();
	}else{
	  (*arch_clt_) << 0;
	}
      }
      // Hunter mode - no wall
      else{
	(*arch_clt_) << 0;
      }
   
    } while (1);
  } catch (SocketException& se) {
    assert(false);
  }

  return gameResult;
}


void Evasion::readSrvUpdateStates(const string& fromSrv) {
  stringstream ss;
  ss << fromSrv;
  // Initialization ; because walls can be created/removed.
  hor_walls_.clear();
  ver_walls_.clear();
  hor_walls_.push_back(new Moveable::Wall(0, 0, 500, 0, 0));
  hor_walls_.push_back(new Moveable::Wall(0, 500, 500, 500, 2));
  ver_walls_.push_back(new Moveable::Wall(0, 0, 0, 500, 1));
  ver_walls_.push_back(new Moveable::Wall(500, 0, 500, 500, 3));

  Moveable::Wall *wall;
  string temp;
  while(!ss.eof()){
    ss >> temp;
    if(temp == "H"){
      int x, y;
      ss >> x;
      ss >> y;
      h_pos.set(x, y);
      h_pos_history_.push_back(h_pos);
    }else if(temp == "P"){
      int x, y;
      ss >> x;
      ss >> y;
      p_pos.set(x, y);
      p_pos_history_.push_back(p_pos);
    }else if(temp == "W" || temp == ","){
      while(true){
	int x1, y1, x2, y2;
	ss >> x1;
	ss >> y1;
	ss >> x2;
	ss >> y2;
	if(y1 == y2){
	  hor_walls_.push_back(new Moveable::Wall(x1, y1, x2, y2, wall_index++));
	}else if(x1 == x2){
	  ver_walls_.push_back(new Moveable::Wall(x1, y1, x2, y2, wall_index++));
	}
	if(ss.get() != ',')
	  break;
      }
    }
  }

  // update bitmap
  for(int i=0; i<=500; i++){
    for(int j=0; j<=500; j++){
      my_obj_->bitmap[i][j].clear();
    }
  }

  // horizontal walls
  for(int k=0; k<hor_walls_.size(); k++){
    int x1, x2, y;
    if(hor_walls_[k]->x1 < hor_walls_[k]->x2){
      x1 = hor_walls_[k]->x1;
      x2 = hor_walls_[k]->x2;
      y = hor_walls_[k]->y1;
    }else{
      x1 = hor_walls_[k]->x2;
      x2 = hor_walls_[k]->x1;
      y = hor_walls_[k]->y1;
    }
    for(int i=x1; i<=x2; i++){
      my_obj_->bitmap[i][y].isWall = true;
    }
  }

  // vertical walls
  for(int k=0; k<ver_walls_.size(); k++){
    int x, y1, y2;
    if(ver_walls_[k]->y1 < ver_walls_[k]->y2){
      y1 = ver_walls_[k]->y1;
      y2 = ver_walls_[k]->y2;
      x = ver_walls_[k]->x1;
    }else{
      y1 = ver_walls_[k]->y2;
      y2 = ver_walls_[k]->y1;
      x = ver_walls_[k]->x1;
    }
    for(int j=y1; j<=y2; j++){
      my_obj_->bitmap[x][j].isWall = true;
    }
  }

  /*
  int tmp, wt, gamePos, ind = 0;
  while ((tmp = ss.get()) != '|');

  while (ss.peek() != '|') {
    ss >> wt;
    assert(ss.get() == int(','));
    ss >> gamePos;

    ++ind;
  }
  */
}

// DFS adj_walls_ matrix graph, find all undirected circles
void Evasion::findWallCircles() {
}

// Register with the architecture server
ClientSocket* Evasion::registerSrv(int srv_port) {
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

void Evasion::dumpAdjWallsMatrixGraph() const {
  for (int i = 0; i < M_ + 4; ++i) {
    for (int j = 0; j < M_ + 4; ++j) {
      std::cout << setw(3) << adj_walls_[i][j];
    }
    std::cout << std::endl;
  }
}
