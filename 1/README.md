# HOMEWORK 1

- Time: 2019-10-16
- Author: lenconda \<[i@lenconda.top](mailto:i@lenconda.top)\>
- OS: macOS 10.15 Catalina
- Uname: `Darwin penghanlindeMacBook-Pro.local 19.0.0 Darwin Kernel Version 19.0.0: Wed Sep 25 20:18:50 PDT 2019; root:xnu-6153.11.26~2/RELEASE_X86_64 x86_64`

## Preamble

It is the final report for the first homework of course `J6110G0004`.

All of the code is written in JavaScript powered by Node.js platforms, and committed to [GitHub Repository](https://github.com/lenconda/operating-system). Code for this homework could be find at [https://github.com/lenconda/operating-system/tree/master/1](https://github.com/lenconda/operating-system/tree/master/1). Besides, the report is written in Markdown, the source could be found at [https://github.com/lenconda/operating-system/blob/master/1/README.md](https://github.com/lenconda/operating-system/blob/master/1/README.md).

## Legal Declaration

- The report is published under [Attribution-NonCommercial-NoDerivatives 4.0](https://creativecommons.org/licenses/by-nc-nd/4.0/) license.

- The code is published and distributed under [MIT](https://opensource.org/licenses/MIT) license.

## Solutions

### Pre Requirements

> Some pigs are trying to cross a ravine. A single rope traverses the ravine, and pigs can cross hand-over-hand. Up to five pigs can be hanging onthe rope at any one time. if there are more than five, then the rope will break and they will all die. Also, if eastward-moving pigs encounter westwardmoving pigs, all will fall off and die.

> Each pig operates in a separate thread, which executes the code below:

```
typedef enum {EAST, WEST} Destination;
void pig(int id, Destination dest) {
  WaitUntilSafeToCross(dest);
  CrossRavine(id, dest);
  DoneWithCrossing(dest);
}
```

In this part, I can create a base class since pigs and students could have the same actions and properties:

```javascript
class GenericModel {
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
```

The constructor defines the id, where to go, reference to the queue, reference to the rope and delay (the duration for instance to destination, usually the same). I set `this.waiting`, `this.crossing`, `this.crossed` to tag every instance's current status.

### Part A & B

Looking like the requirements are so similar between Part A and Part B, I decided to join my answers together. Firstly, I should extend a `Pig` class from `GenericModel`:

```javascript
class Pig extends GenericModel
```

I would like to generate 50 pigs, so I wrote the generator:

```javascript
function generator(id) {
  const destination = Math.round(Math.random()) === 0 ? 'west' : 'east';
  new Pig(id, destination, (destination === 'west' ? toWestQueue : toEastQueue), rope, 5000);
}

for (let i = 0; i < 50; i++) {
  generator(i);
}
```

I used the `Math.random()` in JavaScript to declare where they go, and when the `new Pig()` is called, its constructor will be automatically executed push itself as a `Pig`'s instance to the wait queue.

I hope to add some delay to make the process more observable, so I wrote a delay function using `async/await` feature:

```javascript
async function delay(ms) {
  return new Promise(resolve => {
    setTimeout(() => { resolve(); }, ms);
  });
}
```

Then I went on and wrote the `start()` function:

```javascript
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
```

At the beginning of the function, I also make a random function to choose which side is the first to start, and then go into the `while` loop. I set 500 mile seconds to check if the rope can push new instance(s) or not. Noticing there can only be the pigs who have same destination on the rope, so I have to write a checker function:

```javascript
function checkAvail(destination) {
  for (let i = 0; i < rope.length; i++) {
    if (rope[i].destination !== destination) {
      return false;
    }
  }

  return true;
}
```

The result of the code is like: [pig.txt](https://github.com/lenconda/operating-system/blob/master/1/result/pig.txt)

### Part C

Part C is also similar to Part A and Part B, just modify several conditions:

```javascript
class Students extends GenericModel
```

```javascript
......
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
  ......
}
```

The result of the code is like: [student.txt](https://github.com/lenconda/operating-system/blob/master/1/result/student.txt)
