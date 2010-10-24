#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <guichan.hpp>
#include <guichan/allegro.hpp>
#include <allegro.h>

#include "../hpp/varios.hpp"
#include "../hpp/objeto.hpp"
#include "../hpp/camara.hpp"
#include "../hpp/cenario.hpp"
#include "../hpp/luz.hpp"
#include "../hpp/io.hpp"

class RayTracer {
private:
    char        name[80];
    int         width;
    int         height;
    Camara *    camera;
    Cenario *   scene;
    BITMAP *    buffer;
public:
    RayTracer();
    ~RayTracer();
    
    void setCamera(Vetor_3D position, Vetor_3D direction, Vetor_3D eye);

    BITMAP * run();

    int getWidth() const { return width; }
    int getHeight() const { return height; }
    
    static int makeColor(Cor_rgb color);
};

RayTracer::RayTracer() {
    camera = new Camara();
    scene = new Cenario();
    
    LeArquivoDAT(scene, camera, &width, &height, name);
    LeArquivoPLY(scene, name);

    buffer = create_bitmap(width, height);
    clear_to_color(buffer, makecol(0, 0, 0));
}

RayTracer::~RayTracer() {
    destroy_bitmap(buffer);
    delete camera;
    delete scene;
}

void RayTracer::setCamera(Vetor_3D position, Vetor_3D direction, Vetor_3D eye) {
    camera->Atribui(position, direction, eye);
}

BITMAP * RayTracer::run() {
    Raio ray = camera->PrimeiroRaio();

    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            Cor_rgb cor = scene->Intercepta(ray, 0);
            putpixel(buffer, i, j, RayTracer::makeColor(cor));
            ray = camera->ProximoRaio();
        }
    }

    return buffer;
}

int RayTracer::makeColor(Cor_rgb color) {
    return makecol(color.R(), color.G(), color.B());
}

class VectorInput : public gcn::Container {
private:
    gcn::Label *        label;
    gcn::TextField *    vet[3];
    gcn::Button *       ok;
public:
    VectorInput(const char * name, int width, int height);
    ~VectorInput();
    
    Vetor_3D getVector3D();
    
    void addActionListener(gcn::ActionListener * listener);
    void setCaption(const char * caption);
};

VectorInput::VectorInput(const char * name, int width, int height) {
    setDimension(gcn::Rectangle(0, 0, width, height));

    label = new gcn::Label(name);
    label->setDimension(gcn::Rectangle(0, 0, width/5 -1, height));
    add(label, 0, 0);

    for (int i = 0; i < 3; i++) {
        vet[i] = new gcn::TextField("");
        vet[i]->setDimension(gcn::Rectangle(0, 0, width/5 -1, height));
        add(vet[i], (i+1) * (width/5), 0);
    }

    ok = new gcn::Button("ok");
    ok->setDimension(gcn::Rectangle(0, 0, width/5 -1, height));
    add(ok, 4*width/5, 0);
}

VectorInput::~VectorInput() {
    delete label;
    for (int i = 0; i < 3; i++) delete vet[i];
}

Vetor_3D VectorInput::getVector3D() {
    return Vetor_3D(
        atoi(vet[0]->getText().c_str()),
        atoi(vet[1]->getText().c_str()),
        atoi(vet[2]->getText().c_str()));
}

void VectorInput::addActionListener(gcn::ActionListener * listener) {
    ok->addActionListener(listener);
}

void VectorInput::setCaption(const char * caption) {
    label->setCaption(caption);
}

class App {
private:
    BITMAP *                    display;
    VectorInput *               vectorInput;

    gcn::AllegroInput *         input;
    gcn::AllegroGraphics *      graphics;
    gcn::AllegroImageLoader *   loader;

    gcn::Gui *          gui;
    gcn::Container *    top;
    gcn::ImageFont *    font;
    
    RayTracer * rayTracer;

    int width;
    int height;
public:
    App();
    ~App();
    
    void run();
};

App::App() {    
    allegro_init();
    
    rayTracer = new RayTracer();
    width = rayTracer->getWidth();
    height = rayTracer->getHeight() + 20;

    int bpp = desktop_color_depth();
    set_color_depth(bpp ? bpp : 16);

    set_gfx_mode(GFX_AUTODETECT_WINDOWED, width, height, 0, 0);
    
    display = create_bitmap(width, height);
    
    install_keyboard();
    install_mouse();
    install_timer();

    loader = new gcn::AllegroImageLoader();
    gcn::Image::setImageLoader(loader);
    
    graphics = new gcn::AllegroGraphics();
    graphics->setTarget(display);
    
    input = new gcn::AllegroInput();
    
    font = new gcn::ImageFont("fixedfont.bmp",
        " abcdefghijklmnopqrstuvwxyz"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "0123456789");
    gcn::Widget::setGlobalFont(font);

    gui = new gcn::Gui();
    gui->setGraphics(graphics);
    gui->setInput(input);

    top = new gcn::Container();
    top->setDimension(gcn::Rectangle(0, 0, width, height));
    gui->setTop(top);

    vectorInput = new VectorInput("   Camera   ", width, 20);
    top->add(vectorInput, 0, 0);
}

App::~App() {
    delete rayTracer;

    delete vectorInput;
    delete font;
    delete top;
    delete gui;
  
    delete input;
    delete graphics;
    delete loader;
    
    destroy_bitmap(display);
}

void App::run() {
    while(!key[KEY_ESC]) {
        gui->logic();
        gui->draw();

        blit(rayTracer->run(), display, 0, 0, 0, 20, width, height-20);
        draw_sprite(display, mouse_sprite, mouse_x, mouse_y);
        blit(display, screen, 0, 0, 0, 0, width, height);
    }
}

int main(int argc, char * argv[]) {
    try {
        App app;
        app.run();
    }
    catch(gcn::Exception e) {
        printf("Error: %s\n", e.getMessage().c_str());
    }
    return 0;
}
