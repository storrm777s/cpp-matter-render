Experiment with object attraction in a no gravity environment.  

Tick() is what makes this thing work.

`float Time`: deltaTime, this is also used as a multiplier, as without the added effects, the simulation is very slow.
TimeSetting is an additional multiplier, however it can be controlled by the user.
```
void Tick(float Time) {
    //Time is poorly handled but meh
    Time = Time * (10 * TimeSetting);
    for (auto& asteroid : asteroidlist) {
        asteroid.position.x += asteroid.velocity.x * Time;
        asteroid.position.y += asteroid.velocity.y * Time;

        for (auto& otherasteroid : asteroidlist) {
            if (otherasteroid.id == asteroid.id) {
                continue;
            }
            float distanceX = (otherasteroid.position.x - asteroid.position.x);
            float distanceY = (otherasteroid.position.y - asteroid.position.y);

            float distance = sqrt(distanceX*distanceX + distanceY*distanceY);

            float dx = distanceX / distance;
            float dy = distanceY / distance;

            float acceleration = (1 / (distance * distance + (otherasteroid.mass * 5)) * otherasteroid.mass);

            asteroid.velocity.x += dx * acceleration * Time;
            asteroid.velocity.y += dy * acceleration * Time;
        }
    }
}
```
