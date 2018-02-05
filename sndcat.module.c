package "main";

#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <stdio.h>
#include <poll.h>

#include <sndio.h>

import cards from "cards.module.c";

build append LDFLAGS "-lsndio";
build append CFLAGS "-D_GNU_SOURCE";

int bufsz = 0;
int quit = 0;

void intHandler(int event) {
  fprintf(stderr, "STOP\n");
  quit = 1;
}

void on_move(void * arg, int delta) {
  if (quit == 0) return;

  if (bufsz > 0) {
    bufsz -= delta;
    return;
  }

  struct sio_hdl * mic = (struct sio_hdl *) arg;
  sio_stop(mic);
}


FILE * encode(struct sio_par params, char * out) {
  char * cmd;
  asprintf(&cmd, "opusenc --raw --raw-rate %d --raw-bits %d --raw-chan %d --raw-endianness %d - %s",
        params.rate,
        params.bits,
        params.rchan,
        params.le ? 0 : 1,
        out
  );
  fprintf(stderr, "'%s'\n", cmd);
  return popen(cmd, "w");
}

int main(int argc, char ** argv){
  if (argc < 2) {
    fprintf(stderr, "usage: %s <outfilename.opus>\n", argv[0]);
    return 1;
  }

  char pcm[4096];
  signal(SIGINT, intHandler);
  signal(SIGUSR1, intHandler);
  signal(SIGCHLD, intHandler);

  int card = cards.get_default_input(NULL);
  char device[16];
  sprintf(device, "snd/%d", card);
  struct sio_hdl * mic = sio_open(device, SIO_REC,  0);
  if (mic == NULL) {
    fprintf(stderr, "Unable to open device '%s'\n", device);
    return 1;
  }
  sio_onmove(mic, on_move, mic);

  struct sio_par params = {0};
  sio_getpar(mic, &params);

  fprintf(stderr, "BITS     : %d\n", params.bits);
  fprintf(stderr, "RATE     : %d\n", params.rate);
  fprintf(stderr, "CHANNELS : %d\n", params.rchan);

  bufsz = params.bufsz * 3;

  FILE * encoder = encode(params, argv[1]);

  /* set up poll stuff */
  int nfds = sio_nfds(mic);
  struct pollfd * fds = calloc(nfds, sizeof(struct pollfd));
  sio_pollfd(mic, fds, POLLIN);
  sio_start(mic);

  while (poll(fds, nfds, 2000) > 0) {
    size_t len = sio_read(mic, pcm, 4096);
    fwrite(pcm, 1, len, encoder);
  }

  sio_close(mic);
  pclose(encoder);
  free(fds);
}
