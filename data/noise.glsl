
#version 300 es

uniform lowp float time;
out lowp vec4 fragment;

// A single iteration of Bob Jenkins' One-At-A-Time hashing algorithm.
uint hash( uint x ) {
    x += ( x << 10u );
    x ^= ( x >>  6u );
    x += ( x <<  3u );
    x ^= ( x >> 11u );
    x += ( x << 15u );
    return x;
}



// Compound versions of the hashing algorithm I whipped together.
uint hash( uvec2 v ) { return hash( v.x ^ hash(v.y)                         ); }
uint hash( uvec3 v ) { return hash( v.x ^ hash(v.y) ^ hash(v.z)             ); }
uint hash( uvec4 v ) { return hash( v.x ^ hash(v.y) ^ hash(v.z) ^ hash(v.w) ); }



// Construct a float with half-open range [0:1] using low 23 bits.
// All zeroes yields 0.0, all ones yields the next smallest representable value below 1.0.
float floatConstruct( uint m ) {
    const uint ieeeMantissa = 0x007FFFFFu; // binary32 mantissa bitmask
    const uint ieeeOne      = 0x3F800000u; // 1.0 in IEEE binary32

    m &= ieeeMantissa;                     // Keep only mantissa bits (fractional part)
    m |= ieeeOne;                          // Add fractional part to 1.0

    lowp float  f = uintBitsToFloat( m );       // Range [1:2]
    return f - 1.0;                        // Range [0:1]
}



// Pseudo-random value in half-open range [0:1].
lowp float random( lowp float x ) { return floatConstruct(hash(floatBitsToUint(x))); }
lowp float random( lowp vec2  v ) { return floatConstruct(hash(floatBitsToUint(v))); }
lowp float random( lowp vec3  v ) { return floatConstruct(hash(floatBitsToUint(v))); }
lowp float random( lowp vec4  v ) { return floatConstruct(hash(floatBitsToUint(v))); }


void main()
{
    lowp vec3  inputs = vec3( gl_FragCoord.xy, time ); // Spatial and temporal inputs
    lowp float rand   = random( inputs );              // Random per-pixel value
    lowp vec3  luma   = vec3( rand );                  // Expand to RGB

    gl_FragColor = vec4( luma, 1.0 );
}
