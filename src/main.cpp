#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>
#include <iostream>
#include <vector>
#include <random>

#include "camera.h"
#include "sphere.h"
#include "light.h"
#include "scene.h"

#include "shader.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include <omp.h>

#define SAMPLES_PER_PIXEL 4

struct VectorUchar {
    uint8_t x;
    uint8_t y;
    uint8_t z;
};

void render(uint32_t width, uint32_t height, SDL_Renderer* renderer, Scene* scene, Camera camera) {
    glm::vec2 uvMult = 2.0f / glm::vec2(width, height);

    std::vector<VectorUchar> colors;
    colors.resize(width * height);
    
    #pragma omp parallel for collapse(2)
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            glm::vec3 color(0.0f, 0.0f, 0.0f);
            
            for (int i = 0; i < SAMPLES_PER_PIXEL; i++) {
                float u = x + ((double) rand() / (double) RAND_MAX);
                float v = y + ((double) rand() / (double) RAND_MAX);

                color += scene->raytrace(&camera, glm::vec2(u, v) * uvMult - 1.0f);
            }

            color /= SAMPLES_PER_PIXEL;

            colors[y * width + x].x = (int)(color.x * 255.999);
            colors[y * width + x].y = (int)(color.y * 255.999);
            colors[y * width + x].z = (int)(color.z * 255.999);

            

            /*glm::vec2 uv = glm::vec2(x + 0.5f, y + 0.5f) * uvMult - 1.0f;
            Ray_t ray = camera.CreateRay(uv);
            size_t bestIndex = -1;
            RayHit bestResult;
            bestResult.t = INFINITY;
            for (size_t i = 0; i < objects.size(); ++i) {
                RayHit hitResult;
                if (objects[i].hit(ray, hitResult)) {
                    if (hitResult.t < bestResult.t) {
                        bestResult = hitResult;
                        bestIndex = i;
                    }
                }
            }

            if (bestIndex != -1) {

                glm::vec3 norm = glm::normalize(bestResult.hitNormal);
                glm::vec3 lightDir = glm::normalize(light.m_pLightPos - bestResult.hitPoint);
                
                float diff = glm::max(glm::dot(norm, lightDir), 0.0f);
                glm::vec3 diffuse = light.m_pLightColor * (diff * objects[bestIndex].m_pMaterial.diffuse);

                glm::vec3 viewDir = glm::normalize(ray.origin - bestResult.hitPoint);
                glm::vec3 reflectDir = glm::reflect(-lightDir, norm);
                float spec = glm::pow(glm::max(glm::dot(viewDir, reflectDir), 0.0f), objects[bestIndex].m_pMaterial.shininess);
                glm::vec3 specular = light.m_pLightColor * (spec * objects[bestIndex].m_pMaterial.specular);
    
                glm::vec3 result = (objects[bestIndex].m_pMaterial.ambient * light.m_pLightColor) + diffuse + specular;

                result = glm::clamp(result, 0.0f, 1.0f);
                
                //SDL_SetRenderDrawColor(renderer, (int) (result.x * 255.999), (int) (result.y * 255.999), (int) (result.z * 255.999), SDL_ALPHA_OPAQUE);

                //if (SDL_RenderDrawPoint(renderer, x, y) != 0) {
                    //std::cerr << "IDK" << std::endl;
                //}
                
                //std::cout << (int)(result.x * 255.999)<< ' ' << (int)(result.y * 255.999) << ' ' << (int)(result.z * 255.999) << '\n';
            } else {
                //SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
                //std::cout << 255 << ' ' << 255 << ' ' << 255 << '\n';
            }*/
            
        }
    }

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            VectorUchar color = colors[y * width + x];
            std::cout << (int) color.x << ' ' << (int) color.y << ' ' << (int) color.z << '\n';
        }
    }
}
#include <chrono>


const std::string computeShader = R"(
#version 430
layout(local_size_x = 1, local_size_y = 1) in;
layout(rgba32f, binding = 0) uniform image2D img_output;

void main() {
  // base pixel colour for image
  vec4 pixel = vec4(0.0, 0.0, 0.0, 1.0);
  // get index in global work group i.e x,y position
  ivec2 pixel_coords = ivec2(gl_GlobalInvocationID.xy);
  
  //
  // interesting stuff happens here later
  //
  
  // output to a specific pixel in the image
  imageStore(img_output, pixel_coords, pixel);
}
)";

const std::string fragmentShader = R"(#version 450
out vec4 FragColor;

uniform sampler2D screenTexture;

in vec2 TexCoords;

void main() {
    //FragColor = texture(screenTexture, TexCoords);
    FragColor = vec4(1.0, 0.0, 0.0, 1.0);
})";

const std::string vertexShader = R"(#version 450
layout (location = 0) in vec3 aPos;

out vec2 TexCoords;

void main() {
    gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);
    TexCoords = (aPos.xy+vec2(1,1))/2.0;
})";

GLuint createTexture(uint32_t width, uint32_t height) {
    GLuint tex_output;
glGenTextures(1, &tex_output);
glActiveTexture(GL_TEXTURE0);
glBindTexture(GL_TEXTURE_2D, tex_output);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT,
 NULL);
glBindImageTexture(0, tex_output, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
GLenum e = glGetError();
  if (e != GL_NO_ERROR) {
    fprintf(stderr, "OpenGL error in \"%s\": %d (%d)\n", "Gen Texture", e, e);
    exit(20);
  }
return tex_output;
}

GLuint createQuadVao() {
        static const GLfloat g_quad_vertex_buffer_data[] = {
    -1.0f, -1.0f, 0.0f,
    1.0f, -1.0f, 0.0f,
    -1.0f,  1.0f, 0.0f,
    -1.0f,  1.0f, 0.0f,
    1.0f, -1.0f, 0.0f,
    1.0f,  1.0f, 0.0f,
};
    GLuint quad_vertexbuffer;
glGenBuffers(1, &quad_vertexbuffer);
glBindBuffer(GL_ARRAY_BUFFER, quad_vertexbuffer);
glBufferData(GL_ARRAY_BUFFER, sizeof(g_quad_vertex_buffer_data), g_quad_vertex_buffer_data, GL_STATIC_DRAW);

    GLuint quad_VertexArrayID;
glGenVertexArrays(1, &quad_VertexArrayID);
glBindVertexArray(quad_VertexArrayID);
glBindBuffer(GL_ARRAY_BUFFER, quad_vertexbuffer);
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
glEnableVertexAttribArray(0);


return quad_VertexArrayID;
}
int main() {


    const uint32_t width = 256;
    const uint32_t height = 256;
    const double aspectRatio = width / height;
    
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        fprintf(stderr, "SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return -1;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
 SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    SDL_Window* window = SDL_CreateWindow("Raytracer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

    if (!window) {
        fprintf(stderr, "Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return -1;
    }

    //SDL_GL_SetSwapInterval(1);

    

    auto ctx = SDL_GL_CreateContext(window);

    if (!ctx) {
        std::cout << "Error: " << SDL_GetError() << std::endl;
        return -1;
    }


//SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    //SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);


    glewExperimental = GL_TRUE;
    GLenum err = glewInit();

    if (err != GLEW_OK) {
        std::cout << "Error: " << glewGetErrorString(err) << std::endl;
        return -1;
    }


     
    

    // Set debug callback function
if (GLEW_ARB_debug_output) {
    glDebugMessageCallbackARB(&DebugCallbackARB, nullptr);
}
else if (GLEW_AMD_debug_output) {
    glDebugMessageCallbackAMD(&DebugCallbackAMD, nullptr);
}


//std::cout << "OpenGL Vendor: " << glGetString(GL_VENDOR) << std::endl;
std::cout << "OpenGL Renderer: " << glGetString(GL_RENDERER) << std::endl;
std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << '\n';
std::cout << "OpenGL Shading Language: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << '\n';
std::cout << "OpenGL Max Texture s: " << glGetString(GL_VENDOR) << std::endl;



    SDL_Renderer* renderer;/* = SDL_CreateRenderer(window, -1, 0);

    if (!renderer) {
        fprintf(stderr, "Renderer could not be created! SDL Error: %s\n", SDL_GetError());
        return -1;
    }*/
    //GLuint tex_out = createTexture(width, height);

    //GLuint compute = CreateComputeProgram(computeShader);
    //GLuint quadShader = CreateShaderProgram(vertexShader, fragmentShader);

    //glUseProgram(quadShader);
    //GLuint quadVAO = createQuadVao();

    Material material1(2.0f, glm::vec3(1.0f, 0.5f, 0.01f), glm::vec3(0.5f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
    Material material2(0.02f, glm::vec3(0.30f, 0.15f, 0.75f), glm::vec3(0.75f, 0.25f, 0.5f), glm::vec3(0.1f, 0.2f, 0.25f));

    std::cout << "P3\n" << width << ' ' << height << '\n' << 255 << std::endl;
    glm::vec4 clearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glm::vec3 cameraFront(0.0f, 0.0f, -1.0f);
    glm::vec3 cameraUp(0.0f, 1.0f, 0.0f);
    Sphere sphere(material1, glm::vec3(0.0f, 0.0f, -15.0f), 4.0f);
    glm::vec2 uvMult = 2.0f / glm::vec2(width, height);

    Scene* scene = new Scene();
    
    Camera camera(glm::vec3(0.0f), cameraFront, cameraUp,45.0f, aspectRatio, 0.1f, 100.0f);
    //glm::vec3 sphereColor(1.0f, 0.5f, 0.01f);
    //glm::vec3 sphere2Color(0.0f, 0.5f, 0.5f);
    glm::vec3 lightPos(20.0f, -20.0f, 0.0f);
    glm::vec3 lightColor(0.5f, 0.2f, 0.0f);
    //glm::vec3 diffuseColor(0.5f, 0.0f, 0.0f);
    //glm::vec3 specColor(1.0f, 1.0f, 1.0f);
    //const float shininess = 16.0f;
    float lightPower;


    Light light(lightPos, lightColor);
    scene->addLight(light);
    
    Sphere sphere2(material2, glm::vec3(2.0f, 2.0f, -20.0f), 4.0f);
    std::vector<Sphere> spheres = {sphere, sphere2};
    scene->addObject(&sphere);
    scene->addObject(&sphere2);
    
    auto start = std::chrono::high_resolution_clock::now();
    //SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    //SDL_RenderClear(renderer);
    
    //render(width, height, renderer, scene, camera);
    int indices[] = {0, 1, 2, 3, 4, 5};

    bool isRunning = true;

    while (isRunning) {
        SDL_Event e;
        if (SDL_PollEvent(&e)) {
            switch (e.type) {
                case SDL_KEYDOWN:
                    if (e.key.keysym.sym != SDLK_ESCAPE) continue;
                case SDL_WINDOWEVENT_CLOSE:
                    isRunning = false;
                    break;
            }
        }

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        SDL_GL_SwapWindow(window);


    }
    //for (int i = 0; i < 2000; i++) {
        //{   // Launch compute shaders
            //glUseProgram(compute);
            //glDispatchCompute((GLuint) width, (GLuint) height, 1);
        //}

        // Make sure writing to image has finished before read
        //glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

        //{   // Normal drawing pass
            
            //glUseProgram(quadShader);
            //glBindVertexArray(quadVAO);
            //glActiveTexture(0);
            //glBindTexture(GL_TEXTURE_2D, tex_out);
            
            //glDrawElements(GL_TRIANGLES, 6, GL_FLOAT, indices);
        //}


        //SDL_GL_SwapWindow(window);
        
    //camera.Move(glm::vec3(0.0f, 1.0f, 0.0f));
    //spheres[0].m_pRadius += 0.25f;
    //SDL_RenderPresent(renderer);

    //SDL_Delay(1);
    //}

    auto stop = std::chrono::high_resolution_clock::now();

    delete scene;

    std::cerr << std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count() << std::endl;

    std::cout << std::flush;
    if (renderer) {
        SDL_DestroyRenderer(renderer);
    }
    if (ctx) {
        SDL_GL_DeleteContext(ctx);
    }
        if (window) {
            SDL_DestroyWindow(window);
        }

    SDL_Quit();

    return 0;
}