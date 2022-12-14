# React Notes

## What is a single page application ? 

Usually, when you hit a url you are served the index.html file, which is the root of the application. This file contains 
- html code
- css
- javascript

Now, when you hit the about page in the same website, you would be served about.html. 

Servering new html page files multiple times slows down the front-end. To make it fast, react router intelligently manages different elements on the index.html file based on conditions and url. Due to this, only index.html files is served to the front-end. This is why it is said that react is used to build single page applications.

## What is lazy loading ?

In react, only those parts of the code are rendered which you see on the screen. This is called lazy loading

## Why we use React ? 

- Reusable components

## package.json vs package-lock.json

package-lock.json file is automatically generated by react and managed by npm. This file fixes the versions of the dependencies you use right now so that when you run the app in future, it runs smoothly. You don't make any changes to package-lock.json file. You do an npm upgrade or npm update and that is how you update the package.

## npx

npx means you don't install the package in your local. You just download it, use it and then delete it.

## Dev dependencies

Dev dependencies are not bundled in the production bundle. These are only used by developers to aid in development. It is not included in production to save space.

## npm install -g

You should ideally never use -g flag because it installs packages in your system in global scope so that many react projects can use it. It should not be used because even if you package.json file doesn't contain the dependency you need, it will get imported if it was installed in global scope. If someone else will install the package.json file, then he would realize some dependencies are missing.

## Versioning

major.minor.patch 
- ^ upgrade only minor and patch

## JSX

This is the engine used by react to render your app.

## Virtual DOM

React works on the princple of virtual doms. A virtual DOM is an exact replica of the original DOM. React makes changes to the virtual DOM. Then, the virtual DOM is diffed with the original DOM and only those nodes are updated which have changed.

## let vs const

- let is used for variables that can change
- const is used for variables which cannot be changed

## Spread operator

spread operator flats and adds the properties or elements into another object.

eg

```js
const arr = [1, 2, 3, 4];
const arr1 = [0, ...arr]
console.log(arr1) // output: [0,1,2,3,4]
```

## Unique keys while looping any element

Whenever you are looping over any element in javascript using foreach or map, you need to pass a unique key to JSX so that react can track that element in the virtual dom.

```js
names.map((name, index) => {
return <Card key={`${name}-${index}`} name={name} />
}
```

## Ordering of elements in destructuring

- Ordering matters in destructuring an array because elements are indexed

```js
const [flat, setFlag] = useState(false);
```

- In case of destructuring an object, the ordering doesn't matter

```js
var student = {name: "jatin", age: 26};
var {age, name} = student; // still the correct values would be assigned.
```

## useRef Hook

useRef is used for the following purpose
- If you want to define a variable whose value is preserved between re renders and it doesn't cause re render on changing the value
- If you want to get a reference to a DOM element for the purpose of adding focus etc.

### Things to remember
- useRef returns an object with current property which is the current value of the object. You should always update this current value inside a hook like useEffect to avoid strange behaviour.
- useRef object is only created during mounting of the component and its value is preserved throughout the lifecycle of the component.

## Comparing two objects in javascript
- Two objects in javascript are always compare based on their reference and not their value

```js
const obj1 = {name: "jatin"};
const obj2 = {name: "jatin"};
obj1 === obj2 // this will be false because they would be pointing to differente locations in memory
```

## Stateful and Stateless components
- A component which has no state and is dumb is called a stateless component
- A component which has some interaction with the state is called a stateful component.

## useMemo hook
- useMemo helps you remember the output for a particular intput i.e - it caches it.
- For example, in leetcode the problem statement is converted from markdown to html then displayed on your screen
- markdown --> html conversion is a costly operation. 
- Everytime when we move to discussion and then again open problem statement, this costly conversion would take place. 
- To optimize it, we can use useMemo hook which will memorize the output(html) for the input(markdown) and then use the value from the cache to increase performance.

```js
const doubleNumber = useMemo(()=>slowFunction(input), [input]);

```

## useCallback

- This returns a memoized callback function
- The given function would then not be allocated new memory on every re render.

```js
const onHandleClick = useCallback(e => {
  setInput(e.target.value, 10)
}, []);
```
In the above example, the onHandleClick would be allocated memory only once. 
- useCallback is used for memory intensive functions which don't depend on the state.
- Remember, whenever a component re renders. all the functions are recreeated. So, use useCallback whenever the recreation of a function is unnecessary. 
- For every function you declare, try using useCallback everytime is possible.


## Cleanup in useEffect

- The cleanup function is executed whenever the component is unmounted i.e - it is not visible on the screen


```js
useEffect({} => {
  somefunction();
  
  return () => {
  // cleanup. 
  };
}, []);
```

## Context API
- This is used to avoid prop drilling
- Prop drilling is sending state to lower components so that they can use those.
- Prop drilling should only be used when you need to send the prop to immediate children only.
- Context api creates a data layer to which all the components can talk and get the required data.
- children is a reserved keyword in react. It means components that are present inside the DOM element of the node.

```js
import { createContext } from "react";

const CategoryContext = createContext();
export default function const CategoryProvider({children}) {
return (
  <CategoryContext.Provider>
  {children}
  </CategoryContext.Provider>
)

```











