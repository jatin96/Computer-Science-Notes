# Javascript Basics

## Variables and scoping

```js
console.log('varNum', varNum);
console.log('letNum', letNum);

var varNum = 0;
let letNum = 0;
```

Difference between var and let is that var is hoisted and let is not. Hoisting means that that variable is declarated by js before it is used automatically. In the above example, varNum would show undefined and let would throw error.

Most of the time we should be using let because the roles for let are more natural and easy to understand whereas var can seem ambigous due to hoisting.

```js
const constNum = 0;
let letNum = 0;
const arr = [1,2,3,4];
arr.push(5);
console.log(arr);
```

The above would work because we are not reassigning the arr. We are only pushing a new element into it. 

> Javascript is a very functional language. Most often we try to call functions on values rather than reassign.

## Objects

```js
const myKey = 'key';
const website = {
    name: 'Google',
    rating: 5,
    'multi word key': 0, // multi word key should be enclosed in singe quotes.
    [myKey]: 1234
}
```

### Getting Object Values

```js
website['key'];
website.name // commonly used unless you have to use a variable as a key or a multi word key. 
```

### Inheritance

```js
const website = {
    name: 'Google',
    rating: 5,
    'multi word key': 0, // multi word key should be enclosed in singe quotes.
    [myKey]: 1234
}

const obj = {
    __proto__: website // this is how an object can inherit the properties of another object.
};
```

## Equality and Type Coersion

### Equality
```js
console.log(5 == 5); // loose equality: only compare values
console.log(5 === 5); // strict equality: compare type and values.
console.log('abc' == NaN) // this is false because when abc is coersed to a number, it becomes NaN and NaN is not equal to anything even itself.
```

#### Loose Equality
- If both values are null or undefined, return ```true```
- convert boolean to numbers, true converts to 1 and false to 0
- when string and numbers are compared, convert string to a number
- when comparing object and string, convert object to a string using toString() or valueOf() methods.

> Strict equality should be preferred because it is easy to predict.

> NaN is not equal to anything including itself.

#### Strict Equality
- If either value is NaN, return false.
- If the values have different types, return false.
- If both the values are null or both the values are undefined, return true.
- If both the values are objects, return true of they are the same object else return false.

### Type Coercion

Conversion of an object from one type to another implicitly during equality check or explicit is called type coercion.

> Implicit type conversion: coercion  and explicite type conversion: type casting.

## Syntactic Sugar and Modern Javascript

### Inline functions

Sometimes it's beneficial to use inline functions when you only need the function as a callback or to do some operation.
```js
const arr = [1, 2, 3, 4];

const doubled = arr.map(doubleFunction);

function doubleFunction(int num) {
    return 2 * num;
}

// same code using inline function
const double = arr.map(function(num) {
    return num * 2;
})

// using arrow function
const double = arr.map(num => num * 2);
```

### Destructuring

```js
const [first, second, ...rest] = arr;
```

For objects:
```js
const person = {
    name: 'Jatin',
    age: '27',
    company: 'Foo'
}

const { name } = person;
const {name: firstName, company='Google', ...rest} = person;

function printName({name}) {
    console.log(name);
}

printName(person);
```
Spread operator:

```js
const arr = [1, 2, 3, 4];
function add(x, y) { // only first and second value of arr will be used. 
    console.log(x + y);
}

add(..arr);
```

### Template Literals

```js
console.log(`Hello ${name}`);
```

### Nullish Coalescing

```js
const name = 'Jatin';
const defaultName = name != null ? name: 'Default Name';
// using null coalescing
const defaultName = name ?? 'Default Name';
```

### Optional chaining
Optional chaining prevents runtime errors when you are not sure if a value can be undefined or null. You can use optional chaining to write fallbacks in case it is. In the example below, if the entire expression is undefined or null, we would return 'Foo' and no runtime error would happen.

```js
const person = {
    name: 'jatin'
}

console.log(person?.company?.website ?? 'Foo') // person?.company means that if person.company is undefined instead of throwing error make the whole thing undefined.
```

### Short circuit evaluation

```js
const shouldRunCode = true;

function logWorld() {
    console.log('Hello World');
}

shouldRunCode && logWorld();
```

## Connecting JS to HTML

```<script>``` tag is used to connect js to html.

This tag blocks the browser from rendering the rest of the DOM until the script has finished downloading and executing. There are two ways to change this behavior : 
1. **defer**: Fetch the script asynchronously without blocking the page. Only execute the script after the DOM has finished being parsed
2. **async**: Fetch the script asynchronously without blocking the page. Whenever the script is ready, stop parsing the DOM and start executing the script.

Traditionally, the script tag was placed as the last line of body. This is slower because the script would only be downloaded after the DOM has been parsed. If we use ```defer```, the script would be downloaded asynchronously.

## DOM Manipulation

Using Javascript to change the content, structure or style of the page. There are various properties and functions available to do this.

## Event Driven Programming

 A Pradigm when the code runs in response to events, usually initiated by the user.

 ```js
 button.addEventListner('click', onClick);

 function onClick(event) { // every event listner has a event parameter.
    console.log(event);
 }
 ```

 ### Event propagation

 When you click on a button the control goes from the body to the clicked target in order. This is called capture phase. Once the target is reached, the event is fired. This is called the targetting phase. Post that, the flow goes back up and this is called bubbling phase. In bubbling phase, if you had an event on the parent nodes like body, those will get fired as well.

 ### How to add and eventListner

 ```js
 element.addEventListner(eventName, callback, useCapture); // useCapture means fire the callback during the capture phase rather than the bubbling phase.
 ```

 At any point during propagation, an event listner can call ```event.stopPropagation``` to stop the propagation process.

 ### Common events

 ```js
 window.addEventListner('keydown', event => { // logging key press
    console.log(event.code);
 } )

 scrollable.addEventListner('scrollable',  event => {
    console.log(event.target.scrollTop) // logs how far you have scrolled
 })
 ```

 ### Event Delegation

 The process of using a single event listner on a parent element to manually delegate events to children, rathe than using event listners on each child.

 ## Promises

A promise is an object used for asynchronous operations. These objects are the ones for which we might not know the value yet.
A promise can have three states : pending, fulfilled, rejected.

```js
const promise = new Promise((resolve, reject) => {
    setTimeout(() => reject(new Error('Something went wrong')), 1000);
});

promise.then(value => console.log(value)).catch(error => console.log('Oh no ' + error));

const promise = Promise.resolve(3);

promise
    .then(value => value * 2)
    .then(value => value + 1)
    .then(console.log)
    .catch(error => console.log('Oh no ' + error))
    .finally(() => console.log('Done'))

// Create an array of promises. The array waits for all the promises to be resolved.
Promise.all([Promise.resolve(3), Promise.resolve(4)]);

// Wait for the first fulfilled promise and return

Promise.any([Promise.resolve(3), Promise.resolve(4)]);
```

### Async - Await

```js
async function tester() {
    try {
        const value = await new Promise((resm rej) => setTimeout(() => res(3), 1000));
    } catch(error) {
        console.log('Oh No ' + error);
    }
}
```

## Working With the Server

```js
fetch(BASE_API).then(res => res.text()).then(console.log);
```
```js
JSON.parse() : converts string to JSON object
JSON.stringify() : converts JSON object to string
```

## Closures

Functions have access to all of the variables at the time of definition, even if parent has returned.

> let is block scoped and var is global scoped.

## This

```this``` refers to the context in which the current code is running.

The value of this is determined at runtime
1. At top level, this refers to the global object which is ```window```.
2. In a object method, ```this``` refers to that object.
3. in event listner, the ```this``` will refer to the object it is listening to.
4. Arrow functions do not create their own context for ```this```, so this in an arrow function refers to the context just above it.

### Binding objects to this

```js
function logThis(x, y) {
    console.log(this);
    console.log(x);
    console.log(y);
}

const object = {
    num: 10
};

const boundLogThis = logThis.bind(obj);

boundLogThis(10, 20); // this -> obj

logThis.call(obj, 10, 20) // If you don't want to bind but call the function with an object as this only once.
```

## Class

### Prototypal Inheritance 

The inheritance model used in Javascript. The difference between this type of inheritance and classical inheritance is that prototypal inheritance objects only inherit from other objects, rather than using class blueprints.

```js
const person = {
    isHuman: true
};

const child = Object.create(person) 
/* this will set the __proto__ field to person. In javascript,
 this is called prototypal inheritance.
 We do not have permission to directly edit properties inside of 
 [[Prototype]]. We have few methods exposed which can be used*/

// alternately, we can also do
child.__proto__ = person // this is considered a bad practice however.

// How to copy properties from source to target
const child = Object.assign(Object.create(person), {
    age: 18
});

// Another approach
Object.setPrototypeOf(child, person) // not very well optimized for browsers.

// The prototype property is a reference to the parent object.
const child = Object.create(person);
child.age = 18;

const john = Object.create(child);
john.name = 'John';

console.log(john.age) // gives 18

child.age = 19;

console.log(john.age) // gives 19

// Prototypes of function

const funcProto = Object.getPrototypeOf(() => {});
console.log(Object.getOwnPropertyNames(funcProto)) // this prints all the properties of a function like bind, call, apply etc.

const arrProto = Object.getPrototypeOf([]);
console.log(Object.getOwnPropertyNames(arrProto)); // this prints all the properties of an array like length, find etc.
```

### Function Constructors

A function intended to be used to construct an object using the ```new``` operatior.
 
```js
function Person(name) {
    this.name = name;
}

Person.prototype = {
    constructor: Person,
    isHuman: true // all the objects created with new Person() will have this field as true.
}

const jatin = new Person('Jatin');
```

> ```console.log(jatin instanceof Person) //true. instanceof can be used to check if a const is an instanceof something.``` 

### Polyfill

If some functions are not defined. we can add custom functions using prototype to enhance the functionality of an object.

```js
Array.prototype.push = function(value) { // we have defined our own push method for Array. This is called polyfill.
    this[this.length] = value;
    this.length++;
}
```

### Classes

A javascript syntax to emulate the classic inheritance. This is just syntactic sugar on top of functional constructors so that new developers find it easy to write oop code in Javascript

## Currying

The process of transforming a function to treat its parameters as a sequence of individual function calls that each take one parameter. For example, func(a, b, c) would become func(a)(b)(c)

```js
function sum(a, b, c) {
    return a + b + c;
}

function substract(a, b, c) {
    return a - b - c;
}

function curry(func) {
    return (a) => (b) => (c) => func(a, b, c);
}

const curriedSum = curry(sum);

curriedSum(10)(20)(30);
// currying is mostly not used. But can be useful when you want to save a parameter 

const addFour = curriedSum(4);

console.log(addFour(10)(10)); // no need to add a '4' as parameter in all the calls of addFour
```
















