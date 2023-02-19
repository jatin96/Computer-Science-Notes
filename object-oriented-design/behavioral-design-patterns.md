# Behavioral Design Pattern

## Adapter Pattern

- Let us say you are using Yes Bank apis in your codebase for your UPI payment app. 
- Now, if tomorrow Yes Bank APIs go down, your entire codebase would not work.
- To replace Yes Bank API with ICICI Bank API would be very hard because you have to replace all the files which had Yes Bank api integration.
- RBI has already given that all bank apis must have atleast the standard methods. So different bank can have different name for the apis, but the parameters and returned data would be the same.
- Solution: You upi application should rely on a Generic Bank interface. Entire codebase should refer to the Bank interface. You can then create Different classes which implement Bank interface like YesBank and ICICIBank.
- This way, the only change you need to do now would be to change injection from YesBank to ICICI bank
```java
Bank bank = new YesBank() // before
Bank bank = new ICICIBank() // after
```

## Observer Design Pattern

- Let's say you want to design a flipkart signup service.
- You want to do the following on signup:
    - send a coupon
    - send an OTP
    - subscrive to email campaign
    - update marketing dashboard
- The easiest way is to create different services for the above and inject these services into the sigup class.
- But the above will cause the signup class to depend upon a lot of classes.
- This breaks the open close principle for flipkart class because everytime you have to add a new observer you need to modify flipkart class.
- Observer patterns eliminates the need to modify flipkart class.

```java
public interface SignUpEventObserver {
    void listenSignUpEvent(User user);
}

public class OTPService imnplements SignUpEventObserver {

    public OTPService(Flipkart flipkart) {
        flipkart.registerSignUpEventSubscriber(this);
    }
    public void sendOTP(String phoneNumber) {}

    @Override
    public void listenSignUpEvent(User user) {
        // do listening....
    }
}

class Flipkart { 
    Set<SignUpEventObserver> signUpEventObserver = new HashSet<>();

    void registerSignUpEventObserver(SignUpEventObserver signUpEventObserver) {
        signUpEventObservers.add(signUpEventSubscriber);
    }

    void unregisterSignUpEventObserver(SignUpEventObserver signUpEventObserver) {
        signUpEventObservers.remove(signUpEventObserver);
    }

    public void signUp(User user) {
        for (SignUpEventSubscriber signUpEventSubscriber: signUpEventSubscriber) {
            signUpEventSubscriber.listenSignUpEvent(user);
        }
    }
}

// another alternative is to register the observers in some other class, instead of injection flipkart to all. This would remove direct dependency of observers on flipkart class.
class Runner {
    public static void main(String[] args) {
        Flipkart flipkart = new Flipkart();
        flipkart.registerSignUpEventObserver(new OTPService());
        flipkart.registerSignUpEventObserver();
        flipkart.registerSignUpEventObserver();

    }
}

```