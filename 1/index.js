const Pig = require('./pig');

const toWestQueue = [], toEastQueue = [], rope = [];

function generator(id) {
  const destination = Math.round(Math.random()) === 0 ? 'west' : 'east';
  new Pig(id, destination, (destination === 'west' ? toWestQueue : toEastQueue), rope, 5000);
}

for (let i = 0; i < 50; i++) {
  generator(i);
}

async function delay(ms) {
  return new Promise(resolve => {
    setTimeout(() => { resolve(); }, ms);
  });
}

function checkAvail(destination) {
  for (let i = 0; i < rope.length; i++) {
    if (rope[i].destination !== destination) {
      return false;
    }
  }

  return true;
}

async function start() {
  const destination = Math.round(Math.random()) === 0 ? 'west' : 'east';

  destination === 'west' ? toWestQueue[0].crossRavine() : toEastQueue[0].crossRavine();

  while (toWestQueue.length > 0 || toEastQueue.length > 0) {
    await delay(500);

    if (rope.length <= 4) {
      if ((checkAvail('west') || rope.length === 0) && toWestQueue.length > 0) {
        toWestQueue[0].crossRavine();
      }
      if ((checkAvail('east') || rope.length === 0) && toEastQueue.length > 0) {
        toEastQueue[0].crossRavine();
      }
    }

    console.log(
      JSON.stringify(
        rope
          .map(value => '(' + value.id + ' - ' + value.destination + ')'))
          .split('","')
          .join(' ')
          .replace(/\[\"/, '')
          .replace(/\"\]/, '')
    );
  }
}

start();
