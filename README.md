# SteeringBehaviors
Program displays a window animating different steering behaviors. Built using C++, SFML, and X11.

## Setting up Dependencies
This program was built, compiled, and run on Ubuntu (20.04.3 LTS (GNU/Linux 5.4.0-96-generic x86_64)). Setting up the environment is necessary for the program to build and execute properly. To setup the program environment, run the script <u>setup.sh</u> to create and setup all the dependencies required to run the program.
```
chmod u+x setup.sh
./setup.sh OPEN
```
This will build and run a VM and download all dependencies on that VM required to run the program. Additionally, one can close the VM via the following command:
```
chmod u+x setup.sh
./setup.sh CLOSE
```

## Running the Program
Simply run the script <u>runner.sh</u> with the desired algorithm.
```
chmod u+x runner.sh
./runner.sh <algorithm>
```

Algorithm choices:
* VELOCITY
* ARRIVE
* ALIGN
* WANDER
* FLOCK

## Program Features: Algorithms
This program has a host of steering behavior variable-matching algorithms that can be visualized. See psuedo-code below to know more about these algorithms.

### Algorithm: VelocityMatch
Velocity matching algorithm. This algorithm simply matches the character's velocity to the target's velocity. 
Visually, the character will follow target on parallel path, "copying its movements" but staying a fixed distance away.
```python
def VAR_MATCH_VELOCITY(character, target, timeToTargetVelocity):
    linear = target.velocity - character.velocity
    linear /= timeToTargetVelocity
    return linear
```

### Algorithm: Arrive
Position matching algorithm. This algorithm smoothly moves the character's position to the target's position.
Visually, the character will "arrive" at the target location, smoothly, slowing down upon approach, without oscillation or reversing or overshooting, and finally stopping.
```python
def ARRIVE(character, target, timeToTargetSpeed,
           maxSpeed, radiusOfArrival, radiusOfDeceleration):

    # Extract direction and distance from character to target.
    direction = target.position - character.position
    distance = direction.length()
   
    #  Set goal speed.
    if distance < radiusOfArrival
        goalSpeed = 0
    else if distance > radiusOfDeceleration
        goalSpeed = maxSpeed
    else
        goalSpeed = maxSpeed * (distance / radiusDeceleration)
 
    # Set goal velocity in direction of character to target with goal speed.
    goalVelocity = direction
    goalVelocity.normalize()
    goalVelocity *= goalSpeed
 
    # Make into acceleration such that we can apply and get to goal velocity.
    linear = goalVelocity - character.velocity
    linear /= timeToTargetSpeed
    angular = 0
    return linear, angular
    
```

### Algorithm: Align
Orientation matching algorithm. This algorithm matches character's orientation to target's orientation with no regards to position or velocity of either one. Visually, the character will smoothly orient its direction towards the target over a time, eventually locking.
```python
def mapToRange(rotation):
    r = rotation mod 360
    if |r| <= 180
        return r
    else if |r| > 180
        return 180 - r
    else
        return 180 + r
        
def ALIGN(character, target, timeToReachTargetRotation,
          maxRotation, radiusOfArrival, radiusOfDeceleration): 

    # Extract direction and distance from character to target.
    rotation = target.orientation - character.orientation
    rotation = mapToRange(rotation)
    rotationSize = |rotation|
    
    # Set rotation.
    if rotationSize < radiusOfArrival
        goalRotation = 0
    else if rotationSize > radiusOfDeceleration
        goalRotation = maxRotation
    else
        goalRotation = maxRotation * (rotationSize / radiusOfDeceleration)
    
    # Make into acceleration such that we can apply and get to goal velocity.
    angular = goalRotation - characte.rotation
    angular /= timeToTargetRotation
    linear = 0
    return linear, angular
```

### Algorithm: Wander
This algorithm simply moves character randomly about.
```python
def WANDER(character, target, timeToTarget):
    wanderOrient += randomBinomial * wanderrate
    targetOrient = wanderOrient + character.orient
    target = character.position + wanderOffset + character.orient.asVector()
    target += wanderRadius * targetOrient.asVector()
    result = Face.getSteering()
    linear = maxAcceleration * character.orient.asVector()
    return linear
    
```

### Algorithm: Flock
This algorithm moves group of characters together in unison.
```python
def FLOCK(character, target, timeToTarget):
    // TBD...
```


### Resources
Artifical Intelligence for Games, Third Edition - Ian Millington, ISBN:1138483974
