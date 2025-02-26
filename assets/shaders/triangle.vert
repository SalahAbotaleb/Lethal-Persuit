#version 330

// This vertex shader should be used to render a triangle whose normalized device coordinates are:
// (-0.5, -0.5, 0.0), ( 0.5, -0.5, 0.0), ( 0.0,  0.5, 0.0)
// And it also should send the vertex color as a varying to the fragment shader where the colors are (in order):
// (1.0, 0.0, 0.0), (0.0, 1.0, 0.0), (0.0, 0.0, 1.0)

out Varyings {
    vec3 color;
} vs_out;

// Currently, the triangle is always in the same position, but we don't want that.
// So two uniforms should be added: translation (vec2) and scale (vec2).
// Each vertex "v" should be transformed to be "scale * v + translation".
// The default value for "translation" is (0.0, 0.0) and for "scale" is (1.0, 1.0).

//TODO: (Req 1) Finish this shader
uniform vec2 translation = vec2(0.0,0.0);
uniform vec2 scale = vec2(1.0,1.0);

void main(){

    // Define the vertices of the triangle
    vec3 vertices[3] = vec3[3](
        vec3(-0.5, -0.5, 0.0), // Vertex 1
        vec3( 0.5, -0.5, 0.0), // Vertex 2
        vec3( 0.0,  0.5, 0.0)  // Vertex 3
    );

    // Define the colors for each vertex
    vec3 colors[3] = vec3[3](
        vec3(1.0, 0.0, 0.0), // Color for Vertex 1 (Red)
        vec3(0.0, 1.0, 0.0), // Color for Vertex 2 (Green)
        vec3(0.0, 0.0, 1.0)  // Color for Vertex 3 (Blue)
    );

    // // Default translation vector
    // vec2 translation = vec2(0.0, 0.0);

    // // Default scale vector
    // vec2 scale = vec2(1.0, 1.0);

    // Apply the transformation to the vertex
    vec3 transformedVertex = vec3(scale * vertices[gl_VertexID].xy + translation, vertices[gl_VertexID].z);

    // Pass the color to the fragment shader
    vs_out.color = colors[gl_VertexID];

    // Set the position of the vertex
    gl_Position = vec4(transformedVertex, 1.0);

}