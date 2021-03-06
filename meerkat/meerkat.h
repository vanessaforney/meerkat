#ifndef MEERKAT_H_
#define MEERKAT_H_

#include <vector>
#include <iostream>
#include <cstdlib>
#include <unordered_map>

#include <sys/socket.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <signal.h>
#include <stdio.h>
#include <sys/time.h>

#include "networks.h"
#include "../rpi/GPIO.h"

#define CHILD_PID 0
#define BUDDY_IP 1
#define CALLBACK 2
#define GPIO_STATUS_PIN 4
#define HELP 2
#define NO_PORTS 3
#define ONE_PORT 5
#define BOTH_PORTS 7
#define ERROR -1
#define LOWEST_PORT 10000
#define HIGHEST_PORT 50000
#define DEFAULT_PORT 12345

using namespace std;

enum STATE {
  WAIT_ON_BUDDY, WAIT_ON_DATA
};

enum TYPE {
  LIFE, DEATH
};

typedef struct ports_set {
  uint16_t my_port;
  uint16_t buddy_port;
  TYPE type;
  ports_set(uint16_t p, uint16_t b) {
    my_port = p;
    buddy_port = b;
  }
} ports_set;

// Ensures command line arguments are valid.
ports_set check_args(int argc, char **argv);
// Callback function for a registered signal.
void sigint_handler(int sig);
// Initializes callback routine for handling signals.
void initialize_signal_handler();

class Meerkat;

class Meerkat {
public:
  Meerkat(uint16_t my_port, uint16_t buddy_port, char *buddy_ip, char *callback);

  // Set the socket descriptor of the current meerkat.
  void set_socket_descriptor(int32_t socket_descriptor);

  // Begins the meerkat process to handle incoming connections and initialize
  // the buddy meerkat.
  void process();

  // Add the meerkat to the current meerkat's clan.
  void add_to_clan(sockaddr_in *addr);

private:
  // The port number of the server running the current meerket.
  uint16_t my_port;

  // The port number of the buddy meerket.
  uint16_t buddy_port;

  // The IP address of the buddy meerkat.
  struct sockaddr_in buddy_ip;

  // The name of the program to run.
  char *callback;

  // The map of meerkats that will send a panic message to the current meerkat
  // when they experience loss.
  unordered_map<uint32_t, sockaddr_in> clan;

  // The socket descriptor for the current meerkat.
  int32_t socket_descriptor;

  // The GPIO pin for the meerkat to read from.
  GPIO *gpio;

  // Configure the meerkat and bind to its associated port number.
  void configure();

  // Receives a message from a meerkat in the clan that has initiated loss mode.
  // Calls the user defined callpack function.
  void assist_meerkat();

  // Check the gpio status and sends a loss packet if power is lost.
  void check_gpio_status();

  // The meerkat is waiting for a BUDDY_OK signal for the buddy meerkat.
  STATE wait_on_buddy();

  // The meerkat has received a BUDDY_OK and is waiting to assist other meerkats.
  STATE wait_on_data();
};

#endif
