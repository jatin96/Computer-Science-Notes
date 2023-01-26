# Creational Design Patterns

## Singleton Design Pattern

- Singleton design pattern is used when you need just one object of a class throughout your codebase.
- Singleton pattern is commonly used in loggers, database objects etc.
- A database connection takes time and resources to create. I you use multiple objects of database in your codebase where it is not needed, you would waste resources. Singleton helps you save this resource.
- Singetons are used 
    - In classes which have no attributes. Eg. service, controllers and repositories.
    - Common resource behind the scenes. Eg. logging.
    - The class has attributes, but all of them have the same value in all the use cases.

```java
// Basic Singleton
class BasicSingleton {
    private static BasicSingleton basicSingleton;
    private BasicSingleton() {}

    public static BasicSingleton getInstance() {
        if (basicSingleton == null) {
            basicSingleton = new BasicSingleton();
        }

        return basicSingleton;
    }
}
```
- The above code will fail in a multi threaded environment.
- We are going to use double check locking to make it thread safe.
```java
// Singleton in multithreaded environments (Double check locking)
class MultiThreadedSingleton {
    private static MultiThreadedSingleton singleton;
    private BasicSingleton() {}

    public static MultiThreadedSingleton getInstance() {
        if (singleton == null) {
            synchronized {
                // the below check is neede because the first check is outside synchronized, hence singleton could be made non null by some other thread.
                if (singleton == null) {
                    singleton = new MultiThreadedSingleton();
                }
            }
        }

        return singleton;
    }
}
```
## Builder Design Pattern

- A class has many attributes and you cannot change the value of attributes once the object is created.
- You want some way of creating objects of that class such that you can set value of these attributes during object construction.
- The problem is you would have to define multiple constructors for various combinations of the attributes.

```java
// step 1: create class with its attributes.
class Sandwich {
    private int noOfBreads;
    private int noOfOnions;
    private int noOfPickles;
    // step 2: no constructor and only getters to make class immutable.
    public int getNoOfBreads() {
        return noOfBreads;
    }

    public int getNoOfOnions() {
        return noOfOnions;
    }

    public int getNoOfPickles() {
        return noOfPickles;
    }


    public static Builder newBuilder() {
        return new Builder();
    }

    public static Builder toBuilder() {
        Builder builder = new Builder();
        builder.setNoOfBreads = this.noOfBreads;
        builder.noOfOnions = this.noOfOnions;
        builder.noOfPickles = this.noOfPickles;
        return builder;
    }
    // step 3: create Builder inner class.
    // inner class needs to be static so that you can do Sandwich.
    static class Builder { 
        int noOfBreads;
        int noOfOnions;
        int noOfPickles;

        // step 4: define setters with return type as builder for chaining.
        public Builder setNoOfBreads(int noOfBreads) {
            this.noOfBreads = noOfBreads;
            return this;
        }

        public Builder setNoOfOnions(int noOfOnions) {
            this.noOfOnions = noOfOnions;
            return this;
        }

        public Builder setNoOfPickles(int noOfPickles) {
            this.noOfPickles = noOfPickles;
            return this;
        }

        // step 5: create build method to finally create the object.
        public Sandwich build() {
            Sandwich sandwich = new Sandwich();
            sandwich.noOfBreads = this.noOfBreads;
            sandwich.noOfOnions = this.noOfOnions;
            sandwich.noOfPickles = this.noOfPickles;
            return sandwich;
        }
    }
}    
```

### Uses of Builder Pattern
- You have to create an object of many attributes.
- You have to create immutable objects.
- You need validation before object creation (You can add that noOfBreads > 0 in build before creating the object.)

## Factory Design Pattern
## Prototype Design Pattern


