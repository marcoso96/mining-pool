#ifndef MINER_H_
#define MINER_H_

#include <math.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h> 

#include "packet.h"
#include "workers.h"

#define BUFLEN 512
#define NPACK 10
#define PORT 9930
#define MCAST_GROUP "225.0.0.37"
#define MCAST_PORT 12345
#define HASH_INPUT 20
#define HASH_OUTPUT 20
#define BLOCKSIZE 15
#define MAX_ITER 1
#define MAX_NAME_MINER 20
#define ROBO_BLOQUE 12.5

typedef struct Miner
{
    int minersock;
    int mcastsock;

    // SERVER ADDRESS
    struct hostent *pool;
    struct sockaddr_in pool_addr;
    
    // MULTICAST
    struct sockaddr_in mcast_addr;
    struct ip_mreqn group;

    // MINA'S attributes
    bool active;
    char minerId[MAX_NAME_MINER];

    // Para reconexión
    PacketType_t state;
    
    int32_t nonce;
    int32_t section;
    int32_t curDifficulty;
    char block[BLOCKSIZE];
    
    // TRABAJADORES Y COLA DE TRABAJO
    Worker_t *workers;
    int workerQty;
    mqd_t reqQueue; 
    int count;

    // solo escribo esto cuando lo encontré
    int32_t goldNonce;

    // mi platita
    float wallet;
} Miner_t;

typedef enum 
{
    conn = 0,
    startMining = 1,
    stopMining = 2,
    disc = 3,
    platita = 4,
    closeMiner = 5,
    help = 6,
    helpStart = 7

} MinerInputType_t;

// Creación y destrucción
Miner_t * minerCreate(char * minerId);
void minerDestroy(Miner_t *miner);

// Protocolo de comunicación
void minerInit(Miner_t * miner, char * pool_name);
void minerSendPacket(Miner_t * miner, PacketType_t pType);
void minerProcessPacket(Miner_t * miner);
Packet_t * packetRetrieval(Miner_t *miner);

// Funcionamiento : Inicio y finalización de busqueda de Hash
void minerLoadQueue(Miner_t *miner);
void minerRun(Miner_t *miner);
void minerDecision(Miner_t *miner);
void minerFlush(Miner_t *miner);
void minerExecute(Miner_t * miner, MinerInputType_t inType);
// Funciones útiles
int max(int x, int y);
void hashBlock(void *ctx);

#endif // MINER_H_