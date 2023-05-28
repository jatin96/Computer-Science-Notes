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

