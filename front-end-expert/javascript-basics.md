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


