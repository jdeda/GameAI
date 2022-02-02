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

### Algorithm: VELOCITY
This algorithm simply matches the character's velocity to the target's velocity. 
```python
def VAR_MATCH_VELOCITY(character, target, timeToTargetVelocity):
    linear = target.velocity - character.velocity
    linear /= timeToTargetVelocity
    return linear
```

### Algorithm: ARRIVE
This algorithm smoothly moves character's position to target's position.
```python
def ARRIVE(character, target, timeToTargetSpeed,
           maxSpeed, radiusSatisfaction, radiusDeceleration):

    # Extract direction and distance from character to target.
    direction = target.position - character.position
    distance = direction.length()
   
    # Set goal speed.
    if distance < radiusSatisfaction
        goalSpeed = 0
    else if distance > radiusDeceleration
        goalSpeed = maxSpeed
    else
        goalSpeed = maxSpeed * (distance / radiusDeceleration)
 
    # Set goal velocity.
    goalVelocity = direction
    goalVelocity.normalize()
    goalVelocity *= goalSpeed
 
    # Make into acceleration and return
    linear = goalVelocity - character.velocity
    linear /= timeToTargetSpeed
    return linear, angular # angular probably needed even if not used
    
```

### Algorithm: ALIGN
This algorithm matches character's orientation to target's orientation with no regards to position or velocity of either one.
```python
def mapToRange(rotation):
    r = rotation mod 360
    if |r| <= 180
        return r
    else if |r| > 180
        return 180 - r
    else
        return 180 + r
        
def ALIGN(character, target, timeToTargetRotation,
          maxRotation, radiusSatisfaction, radiusDeceleration): 

    # Extract direction and distance from character to target.
    rotation = target.orientation - character.orientation
    rotation = mapToRange(rotation)
    rotationSize = |rotation|
    
    # Set rotation.
    if rotationSize < radiusSatisfaction
        goalRotation = 0
    else if rotationSize > radiusDeceleration
        goalRotation = maxRotation
    else
        goalRotation = maxRotation * (rotationSize / rotationDecel
        
    
    # Convert to angular acceleration and return.
    angular = goalRotation - characte.rotation
    angular /= timeToTargetRotation
    return angular
```

### Algorithm: WANDER
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

### Algorithm: FLOCK
This algorithm moves group of characters together in unison.
```python
def FLOCK(character, target, timeToTarget):
    // TBD...
```
