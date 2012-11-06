#include <iostream>
#include "jinil_hunter.h"
#include "evasion.h"

using std::cout;
using std::endl;

Jinil_Hunter::~Jinil_Hunter() {
  n_count = 0;
}

void Jinil_Hunter::output() const {
  cout << "Hunter pos: (" << x_ << ", " << y_ << ")" << endl;
}

bool Jinil_Hunter::isHunter() const {
  return true;
}

Moveable::HuntPreyOutput Jinil_Hunter::tryMove() {
  n_count++;
  if(n_count < evade_game_->N_){
    Moveable::HuntPreyOutput ret(0, 0);
    return ret;
  }
  
  // Anticipated Next Prey Position
  updatePosition();
  getHunterDirection();

  st_x = st_y = end_x = end_y = -1;  // Initialization
  algorithm1();

  std::cout << "st_x : " << st_x << ", st_y : " << st_y << std::endl;

  return Moveable::HuntPreyOutput(st_x, st_y, end_x, end_y);
}

void Jinil_Hunter::algorithm1(){
  // You can create wall
  if(evade_game_->m_count < evade_game_->M_){ 
    switch(hunterDirection){
    case LRD_UP:
      if(h_cur.x == p_cur.x-2){
	createWall(h_cur.x, -1);
	return;
      }else if(h_cur.y == p_cur.y-2){
	createWall(-1, h_cur.y);
	return;
      }
	/*
      if(h_cur.x == p_cur.x-2){
	updateTempBitmap(h_cur.x, -1);
	int remainRange = calculateRemainRange();
	int ano_opt_remainRange;  // anticipate another option.
	
	Moveable::Pos h_anti, p_anti;
	h_anti = h_cur;
	p_anti = p_cur;
	while(true){
	  h_anti.x += h_vector_x;
	  h_anti.y += h_vector_y;
	  p_anti.x += p_vector_x;
	  p_anti.y += p_vector_y;
	  if(h_anti.y == p_anti.y-2){
	    updateTempBitmap(-1, h_anti.y);
	    ano_opt_remainRange = calculateRemainRange();
	    break;
	  }
	}

	if(remainRange < ano_opt_remainRange){
	  if(checkCreateWallSafe(h_cur.x, -1)){
	    createWall(h_cur.x, -1);
	    return;
	  }
	}
      }else if(h_cur.y == p_cur.y-2){
	if(checkCreateWallSafe(-1, h_cur.y)){
	  createWall(-1, h_cur.y);
	  return;
	}
      }
	*/
      break;
    case LRD_DOWN:
      if(h_cur.x == p_cur.x-2){
	createWall(h_cur.x, -1);
	return;
      }else if(h_cur.y == p_cur.y-2){
	createWall(-1, h_cur.y);
	return;
      }
      /*
      if(h_cur.y == p_cur.y-2){
	updateTempBitmap(-1, h_cur.y);
	int remainRange = calculateRemainRange();
	int ano_opt_remainRange;  // anticipate another option.
	Moveable::Pos h_anti, p_anti;
	h_anti = h_cur;
	p_anti = p_cur;
	while(true){
	  h_anti.x += h_vector_x;
	  h_anti.y += h_vector_y;
	  p_anti.x += p_vector_x;
	  p_anti.y += p_vector_y;
	  if(h_anti.x >= p_anti.x-2){
	    updateTempBitmap(h_anti.x, -1);
	    ano_opt_remainRange = calculateRemainRange();
	    break;
	  }
	}
	if(remainRange < ano_opt_remainRange){
	  if(checkCreateWallSafe(-1, h_cur.y)){
	    createWall(-1, h_cur.y);
	    return;
	  }
	}
      }else if(h_cur.x == p_cur.x-2){
	if(checkCreateWallSafe(h_cur.x, -1)){
	  createWall(h_cur.x, -1);
	  return;
	}
      }
      */
      break;
    case RLU_UP:
      if(h_cur.x == p_cur.x+2){
	createWall(h_cur.x, -1);
	return;
      }else if(h_cur.y == p_cur.y+2){
	createWall(-1, h_cur.y);
	return;
      }
      
      /*
      if(h_cur.y == p_cur.y+2){
	updateTempBitmap(-1, h_cur.y);
	int remainRange = calculateRemainRange();
	int ano_opt_remainRange;  // anticipate another option.
	Moveable::Pos h_anti, p_anti;
	h_anti = h_cur;
	p_anti = p_cur;
	while(true){
	  h_anti.x += h_vector_x;
	  h_anti.y += h_vector_y;
	  p_anti.x += p_vector_x;
	  p_anti.y += p_vector_y;
	  if(h_anti.x == p_anti.x+2){
	    updateTempBitmap(h_anti.x, -1);
	    ano_opt_remainRange = calculateRemainRange();
	    break;
	  }
	}
	if(remainRange < ano_opt_remainRange){
	  if(checkCreateWallSafe(-1, h_cur.y)){
	    createWall(-1, h_cur.y);
	    return;
	  }
	}
      }else if(h_cur.x == p_cur.x+2){
	if(checkCreateWallSafe(h_cur.x, -1)){
	  createWall(h_cur.x, -1);
	  return;
	}
      }
      */
      break;
    case RLU_DOWN:
      if(h_cur.x == p_cur.x+2){
	createWall(h_cur.x, -1);
	return;
      }else if(h_cur.y == p_cur.y+2){
	createWall(-1, h_cur.y);
	return;
      }
      /*
      if(h_cur.x == p_cur.x+2){
	updateTempBitmap(h_cur.x, -1);
	int remainRange = calculateRemainRange();
	int ano_opt_remainRange;  // anticipate another option.
	Moveable::Pos h_anti, p_anti;
	h_anti = h_cur;
	p_anti = p_cur;
	while(true){
	  h_anti.x += h_vector_x;
	  h_anti.y += h_vector_y;
	  p_anti.x += p_vector_x;
	  p_anti.y += p_vector_y;
	  if(h_anti.y == p_anti.y+2){
	    updateTempBitmap(-1, h_anti.y);
	    ano_opt_remainRange = calculateRemainRange();
	    break;
	  }
	}
	if(remainRange < ano_opt_remainRange){
	  if(checkCreateWallSafe(h_cur.x, -1)){
	    createWall(h_cur.x, -1);
	    return;
	  }
	}
      }else if(h_cur.y == p_cur.y+2){
	if(checkCreateWallSafe(-1, h_cur.y)){
	  createWall(-1, h_cur.y);
	  return;
	}
      }
      */
      break;
    case LRU_UP:
      if(h_cur.x == p_cur.x-2){
	createWall(h_cur.x, -1);
	return;
      }else if(h_cur.y == p_cur.y+2){
	createWall(-1, h_cur.y);
	return;
      }
      /*
      if(h_cur.y == p_cur.y+2){
	updateTempBitmap(-1, h_cur.y);
	int remainRange = calculateRemainRange();
	int ano_opt_remainRange;  // anticipate another option.
	Moveable::Pos h_anti, p_anti;
	h_anti = h_cur;
	p_anti = p_cur;
	while(true){
	  h_anti.x += h_vector_x;
	  h_anti.y += h_vector_y;
	  p_anti.x += p_vector_x;
	  p_anti.y += p_vector_y;
	  if(h_anti.x == p_anti.x-2){
	    updateTempBitmap(h_anti.x, -1);
	    ano_opt_remainRange = calculateRemainRange();
	    break;
	  }
	}
	if(remainRange < ano_opt_remainRange){
	  if(checkCreateWallSafe(-1, h_cur.y)){
	    createWall(-1, h_cur.y);
	    return;
	  }
	}
      }else if(h_cur.x == p_cur.x-2){
	if(checkCreateWallSafe(h_cur.x, -1)){
	  createWall(h_cur.x, -1);
	  return;
	}
      }
      */
      break;
    case LRU_DOWN:
      if(h_cur.x == p_cur.x-2){
	createWall(h_cur.x, -1);
	return;
      }else if(h_cur.y == p_cur.y+2){
	createWall(-1, h_cur.y);
	return;
      }
      /*
      if(h_cur.x == p_cur.x-2){
	updateTempBitmap(h_cur.x, -1);
	int remainRange = calculateRemainRange();
	int ano_opt_remainRange;  // anticipate another option.
	Moveable::Pos h_anti, p_anti;
	h_anti = h_cur;
	p_anti = p_cur;
	while(true){
	  h_anti.x += h_vector_x;
	  h_anti.y += h_vector_y;
	  p_anti.x += p_vector_x;
	  p_anti.y += p_vector_y;
	  if(h_anti.y == p_anti.y+2){
	    updateTempBitmap(-1, h_anti.y);
	    ano_opt_remainRange = calculateRemainRange();
	    break;
	  }
	}
	if(remainRange < ano_opt_remainRange){
	  if(checkCreateWallSafe(h_cur.x, -1)){
	    createWall(h_cur.x, -1);
	    return;
	  }
	}
      }else if(h_cur.y == p_cur.y+2){
	if(checkCreateWallSafe(-1, h_cur.y)){
	  createWall(-1, h_cur.y);
	  return;
	}
      }
      */
      break;
    case RLD_UP:
      if(h_cur.x == p_cur.x+2){
	createWall(h_cur.x, -1);
	return;
      }else if(h_cur.y == p_cur.y-2){
	createWall(-1, h_cur.y);
	return;
      }
      /*
      if(h_cur.x == p_cur.x+2){
	updateTempBitmap(h_cur.x, -1);
	int remainRange = calculateRemainRange();
	int ano_opt_remainRange;  // anticipate another option.
	Moveable::Pos h_anti, p_anti;
	h_anti = h_cur;
	p_anti = p_cur;
	while(true){
	  h_anti.x += h_vector_x;
	  h_anti.y += h_vector_y;
	  p_anti.x += p_vector_x;
	  p_anti.y += p_vector_y;
	  if(h_anti.y == p_anti.y-2){
	    updateTempBitmap(-1, h_anti.y);
	    ano_opt_remainRange = calculateRemainRange();
	    break;
	  }
	}
	if(remainRange < ano_opt_remainRange){
	  if(checkCreateWallSafe(h_cur.x, -1)){
	    createWall(h_cur.x, -1);
	    return;
	  }
	}
      }else if(h_cur.y == p_cur.y-2){
	if(checkCreateWallSafe(-1, h_cur.y)){
	  createWall(-1, h_cur.y);
	  return;
	}
      }
      */
      break;
    case RLD_DOWN:
      if(h_cur.x == p_cur.x+2){
	createWall(h_cur.x, -1);
	return;
      }else if(h_cur.y == p_cur.y-2){
	createWall(-1, h_cur.y);
	return;
      }
      /*
      if(h_cur.y == p_cur.y-2){
	updateTempBitmap(-1, h_cur.y);
	int remainRange = calculateRemainRange();
	int ano_opt_remainRange;  // anticipate another option.
	Moveable::Pos h_anti, p_anti;
	h_anti = h_cur;
	p_anti = p_cur;
	while(true){
	  h_anti.x += h_vector_x;
	  h_anti.y += h_vector_y;
	  p_anti.x += p_vector_x;
	  p_anti.y += p_vector_y;
	  if(h_anti.x == p_anti.x+2){
	    updateTempBitmap(h_anti.x, -1);
	    ano_opt_remainRange = calculateRemainRange();
	    break;
	  }
	}
	if(remainRange < ano_opt_remainRange){
	  if(checkCreateWallSafe(-1, h_cur.y)){
	    createWall(-1, h_cur.y);
	    return;
	  }
	}
      }else if(h_cur.x == p_cur.x+2){
	if(checkCreateWallSafe(h_cur.x, -1)){
	  createWall(h_cur.x, -1);
	  return;
	}
      }
      */
      break;
    }
  }

  // If you come here,
  // you can remove wall
  // Check removable wall
  int index = checkRemovableWall();
  if(index != -1)
    removeWall(index);
  
}

void Jinil_Hunter::createWall(int x, int y){

  std::cout << "createWall_test1" << std::endl;

  vector<Moveable::Wall*> hor_walls = evade_game_->hor_walls_;
  vector<Moveable::Wall*> ver_walls = evade_game_->ver_walls_;
  st_x = end_x = h_cur.x;
  st_y = end_y = h_cur.y;
  bool isContinue = true;

  if(x != -1){  // vertical wall
    
    std::cout << "createWall_test2" << std::endl;

    while(isContinue){
      isContinue = false;
      if(st_y-1 >=0 && !temp_bitmap[st_x][st_y-1].isWall){
	st_y--;
	isContinue = true;
      }
      if(end_y+1 <=500 && !temp_bitmap[st_x][end_y+1].isWall){
	end_y++;
	isContinue = true;
      }
    }

    std::cout << "createWall_test3" << std::endl;

  }else{        // horizontal wall

    std::cout << "createWall_test4" << std::endl;

    while(isContinue){
      isContinue = false;
      if(st_x-1 >=0 && !temp_bitmap[st_x-1][st_y].isWall){
	st_x--;
	isContinue = true;
      }
      if(end_x+1 <=500 && !temp_bitmap[end_x+1][st_y].isWall){
	end_x++;
	isContinue = true;
      }
    }

    std::cout << "createWall_test5" << std::endl;

  }

  std::cout << "createWall_test6" << std::endl;
  
  evade_game_->m_count++;
  n_count = 0;
}

void Jinil_Hunter::removeWall(int index){
  vector<Moveable::Wall*> hor_walls = evade_game_->hor_walls_;
  vector<Moveable::Wall*> ver_walls = evade_game_->ver_walls_;
  bool find = false;
  for(int k=2; k<hor_walls.size(); k++){
    if(hor_walls[k]->wid_ == index){
      st_x = hor_walls[k]->x1;
      st_y = hor_walls[k]->y1;
      end_x = hor_walls[k]->x2;
      end_y = hor_walls[k]->y2;
      find = true;
      break;
    }
  }
  if(find)
    goto removeWall_end;

  for(int k=2; k<ver_walls.size(); k++){
    if(ver_walls[k]->wid_ == index){
      st_x = ver_walls[k]->x1;
      st_y = ver_walls[k]->y1;
      end_x = ver_walls[k]->x2;
      end_y = ver_walls[k]->y2;
      break;
    }
  }

 removeWall_end:
  evade_game_->m_count--;
  n_count = 0;
}

int Jinil_Hunter::checkRemovableWall(){
  vector<Moveable::Wall*> hor_walls = evade_game_->hor_walls_;
  vector<Moveable::Wall*> ver_walls = evade_game_->ver_walls_;
  int index = -1;
  for(int k=2; k<hor_walls.size(); k++){
    int x1, x2, y;
    if(hor_walls[k]->x1 <= hor_walls[k]->x2){
      x1 = hor_walls[k]->x1;
      x2 = hor_walls[k]->x2;
      y = hor_walls[k]->y1;
    }else{
      x1 = hor_walls[k]->x2;
      x2 = hor_walls[k]->x1;
      y = hor_walls[k]->y1;
    }
    bool removable = false;
    for(int i=x1; i<=x2; i++){
      if(y-1>=0 && temp_bitmap[i][y-1].isAvail){
	removable = true;
	break;
      }
      if(y+1<=500 && temp_bitmap[i][y+1].isAvail){
	removable = true;
	break;
      }
    }
    if(removable){
      index = hor_walls[k]->wid_;
      break;
    }
  }
  if(index != -1)
    return index;

  for(int k=2; k<ver_walls.size(); k++){
    int x, y1, y2;
    if(ver_walls[k]->y1 <= ver_walls[k]->y2){
      y1 = ver_walls[k]->y1;
      y2 = ver_walls[k]->y2;
      x = ver_walls[k]->x1;
    }else{
      y1 = ver_walls[k]->y2;
      y2 = ver_walls[k]->y1;
      x = ver_walls[k]->x1;
    }
    bool removable = false;
    for(int j=y1; j<=y2; j++){
      if(x-1>=0 && temp_bitmap[x-1][j].isAvail){
	removable = true;
	break;
      }
      if(x+1<=500 && temp_bitmap[x+1][j].isAvail){
	removable = true;
	break;
      }
    }
    if(removable){
      index = ver_walls[k]->wid_;
      break;
    }
  }
  return index;
}

int Jinil_Hunter::calculateRemainRange(){
  int sum = 0;
  for(int i=0; i<=500; i++){
    for(int j=0; j<=500; j++){
      if(temp_bitmap[i][j].isAvail)
	sum++;
    }
  }
  return sum;
}

// If x=-1 & y=-1, there are no virtual wall
void Jinil_Hunter::updateTempBitmap(int x, int y){
  // copy
  for(int i=0; i<=500; i++){
    for(int j=0; j<=500; j++){
      temp_bitmap[i][j] = bitmap[i][j];
      temp_bitmap[i][j].isAvail = false;
    }
  }

  // Set virtual wall : for calculation before create wall
  if(x != -1){
    for(int j=0; j<=500; j++)
      temp_bitmap[x][j].isWall = true;
  }else{
    for(int i=0; i<=500; i++)
      temp_bitmap[i][y].isWall = true;
  }

  // fill available area
  int max_left, max_right;
  int i, j;
  temp_bitmap[p_cur.x][p_cur.y].isAvail = true;   // prey position is true
  for(i=p_cur.x+1; i<=500; i++){  // prey ~ right wall
    if(temp_bitmap[i-1][p_cur.y].isAvail && !temp_bitmap[i][p_cur.y].isWall)
      temp_bitmap[i][p_cur.y].isAvail = true;
    if(temp_bitmap[i][p_cur.y].isWall)
      break;
  }
  max_right = i;
  for(i=p_cur.x-1; i>=0; i--){  // left wall ~ prey
    if(temp_bitmap[i+1][p_cur.y].isAvail && !temp_bitmap[i][p_cur.y].isWall)
      temp_bitmap[i][p_cur.y].isAvail = true;
    if(temp_bitmap[i][p_cur.y].isWall)
      break;
  }
  max_left = i;

  for(i=max_left; i<=max_right; i++){  // top wall ~ prey
    for(j=p_cur.y-1; j>=0; j--){
      if(temp_bitmap[i][j+1].isAvail && !temp_bitmap[i][j].isWall)
	temp_bitmap[i][j].isAvail = true;
      if(temp_bitmap[i][j].isWall)
	break;
    }
  }
  for(i=max_left; i<=max_right; i++){  // prey ~ bottom wall
    for(j=p_cur.y+1; j<=500; j++){
      if(temp_bitmap[i][j-1].isAvail && !temp_bitmap[i][j].isWall)
	temp_bitmap[i][j].isAvail = true;
      if(temp_bitmap[i][j].isWall)
	break;
    }
  }

  // Optional check : nomally this part is not needed.
  // left to right
  for(j=0; j<=500; j++){
    for(i=1; i<=500; i++){
      if(temp_bitmap[i-1][j].isAvail && !temp_bitmap[i][j].isWall)
	temp_bitmap[i][j].isAvail = true;
    }
  }
  // right to left
  for(j=0; j<=500; j++){
    for(i=499; i>=0; i--){
      if(temp_bitmap[i+1][j].isAvail && !temp_bitmap[i][j].isWall)
	temp_bitmap[i][j].isAvail = true;
    }
  }
  // top to bottom
  for(i=0; i<=500; i++){
    for(j=1; j<=500; j++){
      if(temp_bitmap[i][j-1].isAvail && !temp_bitmap[i][j].isWall)
	temp_bitmap[i][j].isAvail = true;
    }
  }
  // bottom to top
  for(i=0; i<=500; i++){
    for(j=499; j>=0; j--){
      if(temp_bitmap[i][j+1].isAvail && !temp_bitmap[i][j].isWall)
	temp_bitmap[i][j].isAvail = true;
    }
  }
}

bool Jinil_Hunter::checkCreateWallSafe(int x, int y){
  // Check squiz
  // If the next hunter's position is bounced
  // it should be squized. 
  if(x != -1){     // vertical wall
    if(h_cur.x == h_next.x)
      return false;
  }else{           // horizontal wall
    if(h_cur.y == h_next.y)
      return false;
  }
  return true;

  /*
  if(temp_bitmap[h_next.x][h_next.y].isAvail)
    return true;
  else
    return false;
  */
}

void Jinil_Hunter::updatePosition(){
  vector<Pos> h_pos_history = evade_game_->h_pos_history_;
  vector<Pos> p_pos_history = evade_game_->p_pos_history_;
  // Current position
  h_cur = evade_game_->h_pos;
  p_cur = evade_game_->p_pos;

  // Past position
  if(h_pos_history.size()>=3){
    h_past = h_pos_history[ h_pos_history.size()-2 ];
    h_past_past = h_pos_history[ h_pos_history.size()-3 ];
  }else{
    h_past.x = h_cur.x-1;
    h_past.y = h_cur.y-1;
    h_past_past.x = h_cur.x-2;
    h_past_past.y = h_cur.x-2;
  }
  if(p_pos_history.size()>=2)
    p_past = p_pos_history[ p_pos_history.size()-2];
  else
    p_past = p_cur;
  
  // Vector
  if(h_cur.x == h_past.x || h_cur.y == h_past.y){ // Just previously bounced
    if(h_cur.x == h_past.x){   // Just previously bounced on a vertical wall
      if(h_past.x > h_past_past.x)
	h_vector_x = -1;
      else if(h_past.x < h_past_past.x)
	h_vector_x = 1;
    }
    if(h_cur.y == h_past.y){    // Just previously bounced on a horizontal wall
      if(h_past.y > h_past_past.y)
	h_vector_y = -1;
      else if(h_past.y < h_past_past.y)
	h_vector_y = 1;
    }
  }else{ // not bounced on a wall
    h_vector_x = h_cur.x - h_past.x;
    h_vector_y = h_cur.y - h_past.y;
  }

  p_vector_x = p_cur.x - p_past.x;
  p_vector_y = p_cur.y - p_past.y;

  // anticipated next prey's position
  // next = current + (current - past)       // Need more specific, especially reflex on the wall
  h_next = getNextHunterPosition(1);
  p_next.x = p_cur.x + p_vector_x;
  p_next.y = p_cur.y + p_vector_y;
}

void Jinil_Hunter::getHunterDirection(){
  
  if(h_vector_x>0 && h_vector_y>0){
    if(h_cur.y > p_cur.y || h_cur.x > p_cur.x)
      hunterDirection = LRD_UP;
    else
      hunterDirection = LRD_DOWN;
  }else if(h_vector_x<0 && h_vector_y<0){
    if(h_cur.y > p_cur.y || h_cur.x > p_cur.x)
      hunterDirection = RLU_UP;
    else
      hunterDirection = RLU_DOWN;
  }else if(h_vector_x>0 && h_vector_y<0){
    if(h_cur.y > p_cur.y || h_cur.x < p_cur.x)
      hunterDirection = LRU_UP;
    else
      hunterDirection = LRU_DOWN;
  }else if(h_vector_x<0 && h_vector_y>0){
    if(h_cur.y > p_cur.y || h_cur.x < p_cur.x)
      hunterDirection = RLD_UP;
    else
      hunterDirection = RLD_DOWN; 
  }
}


