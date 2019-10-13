const { Student } = require('./models');

const toWestQueue = [], toEastQueue = [], rope = [];

function generator(id) {
  const destination = Math.round(Math.random()) === 0 ? 'west' : 'east';
  new Student(id, destination, (destination === 'west' ? toWestQueue : toEastQueue), rope, 5000);
}

for (let i = 0; i < 50; i++) {
  generator(i);
}

async function delay(ms) {
  return new Promise(resolve => {
    setTimeout(() => { resolve(); }, ms);
  });
}

function checkCount(destination, count) {
  const currentCount = rope.filter(value => value.destination === destination).length;
  return currentCount < count;
}

async function start() {
  const destination = Math.round(Math.random()) === 0 ? 'west' : 'east';

  destination === 'west' ? toWestQueue[0].crossRavine() : toEastQueue[0].crossRavine();

  while (toWestQueue.length > 0 || toEastQueue.length > 0) {
    await delay(500);

    if (rope.length <= 9) {
      if ((checkCount('west', 4) || rope.length === 0) && toWestQueue.length > 0) {
        toWestQueue[0].crossRavine();
      }
      if ((checkCount('east', 6) || rope.length === 0) && toEastQueue.length > 0) {
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
