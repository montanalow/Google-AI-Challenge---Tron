/*
 * File:   MyTronBot.cc
 * Author: montana
 *
 * Created on February 8, 2010, 2:49 PM
 */

#include "bot.h"

int main() {
  zz::bot bot;
  while (true) {
    bot.set_map(stdin);
    bot.move();
  }
  return 0;
}

namespace zz{

}