class Pig {
  constructor(id, destination, queue, rope, delay) {
    this.id = id;
    this.destination = destination;
    this.waiting = true;
    this.crossing = false;
    this.crossed = false;
    this.queue = queue;
    this.rope = rope;
    this.delay = delay;
    this.queue.push(this);
  }

  waitUntilSafeToCross() {
    this.waiting = true;
    this.crossing = false;
    this.crossed = false;
  }

  crossRavine() {
    this.waiting = false;
    this.crossing = true;
    this.crossed = false;
    this.queue.shift();
    this.rope.push(this);
    setTimeout(() => this.doneWithCrossing(), this.delay);
  }

  doneWithCrossing() {
    this.waiting = false;
    this.crossing = false;
    this.crossed = true;
    this.rope.shift();
  }
}

exports = module.exports = Pig;
