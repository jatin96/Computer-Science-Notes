# Structural Design Patterns

- These focus on how to structure the objects.

## Adapter Pattern
- This design pattern helps you to mould or adapt the data from one format to another.
- Class adapters are usually used to mould external apis to our application's use. In other words, if we are using some third party apis, we should use an adapter to abstract them.
- This makes you independent of the third party apis going down. If the current bank provider goes down, you can create a provider or adapter for a new one and inject that instead.

```java
interface BankProvider {
    void makePayment(UPIId from, UPIId to, Double amount);
    double checkBalance(UPIId account);
}

class YesBankProvider implements BankProvider {
    void makePayment(UPIId from, UPIId to, double amount) {
        // call yes banks api here
    }

    double checkBalance(UPIInd account) {
        // call yes bank's api here.
    }
}

class HDFCBankProvider implements BankProvider {
    void makePayment(UPIId from, UPIId to, double amount) {
        // call hdfc banks api here
    }

    double checkBalance(UPIInd account) {
        // call hdfc bank's api here.
    }
}

public static void main() {
    BankProvider bankprovider = new YesBankProvider();
}
```

## Decorator Design Pattern

- Decorator design pattern creates a decorator around an object.
- The purpose of wrapping an object is to add functionality to it.
- It is different from a builder because in a builder it will always create a new object. In decorator you already have an object to which you want to add some functionality.

```java
class Pizza {
    public Pizza(PizzaBase base) {

    }
}

class MeatDecorator {
    public static Pizza addSausages(Pizza pizza, int amount) {
        // add sausages and return the pizza.
    }
}

class Dominos {
    void placeOrder(String orderDetails) {
        Pizza pizza = new Pizza(PizzaBaseFactory.getBase());
        MeatDecorator.addSausages(pizza, 10);
    }
}
```
```java
class HTMLDecorators {
    public static String center(String html) {
        return "<div style='margin: auto; width: 60%'>" + html + "</div>";
    }

    public static String makeBold(String html) {
        return "<b>" + html + "</b>";
    }

    public static String span(String html) {
        ...
    }
}
```

## Flyweight Design Pattern

- Many times we end up creating classes with two types of attributes
    - **Intrinsic attributes:** That remain same for all objects.
    - **Extrinsic attributes:** That change based on external factors.
- Let's say you are creating a game where you have bullets. Each bullet can have the following attributes
    - Intrinsic attributes : image, damage, type.
    - Extrinsic attributes: direction, currentSpeed.
- Now if you create a bullet class and give all the attributes inside it, it will take a lot of space. Since games work in RAM, you need to optimize storage.
- The idea is to create a separate class called Bullet with all the intrinsic properties, and create a class called FlyingBullet with all the extrinsic properties.
- FlyingBullet will also have a attribute called bullet whose value will be injected using a constructor. 
- Since you are using only a reference here, the size would be saved because you need not create multiple copies of bullet.

```java
class BulletType {
    FIVE,
    SEVEN,
    ZERO,
    NINE,
}

class BulletRegistry {
    Map<BulletType, Bullet> bullets = new HashMap<>();

    void register(BulletType type, Bullet bullet) {

    }

    Bullet get(BulletType type) {

    }
}

// immutable objects of intrinsic class often stored in a registry.
class Bullet {
    BulletType type;
    int maxDamage;
    byte[] image;
    double weight;
}

class FlyingBullet { 
    Bullet bullet;
    double curSpeed;
    double curPosX;
    double curPosY;
    double curPosZ;
    double curDirection;
    boolean isFired;
}

class Client {
    public static void main(String[] args) {
        List<Bullet> bullets = new ArrayList<>();
        BulletRegistry registry = new BulletRegistry();
        Bullet fiveMMBullet = new Bullet();
        fiveMMBullet.maxDamage = 300;
        fiveMMBullet.weight = 200;
        registry.register(BulletType.FIVE, fiveMMBullet);

        for(int i = 0; i < 10000; i++) {
            FlyingBullet flyingBullet = new FlyingBullet();
            String type = "5"; // take input from the cli
            flyingBullet.bullet = registry.get(BulletType.FIVE);
        }

    }
}
```
>  "Flyweight" pattern gets is name from the fact that often there are a lot of flying classes (classes whose parameters are constantly changing) and in such classes some attributes are fixed which add a lot of weight to class, thus making it difficult to fly.

- Another example could be a Zerodha. Zerodha has stock listings and for a stock there are some intrinsic attributes like name, etc. and some extrinsic attributes like curPrice etc. So we can use flyweight pattern there.
