#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <guichan.hpp>
#include <guichan/allegro.hpp>
#include <allegro.h>

#include "../hpp/define.hpp"
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
	CaixaParalela *box; //Deve se tornar uma matriz, mas aí eu me emblo todo... Amanhã a gnt vê isso junto!
public:
    RayTracer();
    ~RayTracer();
    
    void setCameraPosition(Vetor_3D position);
    void setCameraDirection(Vetor_3D direction);
    void setCameraEye(Vetor_3D eye);

    void setCamera(Vetor_3D position, Vetor_3D direction, Vetor_3D eye);

    void run();
    
    BITMAP * getBuffer() const { return buffer; }
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    
    static int makeColor(Cor_rgb color);
};

RayTracer::RayTracer() {
    camera = new Camara();
    scene = new Cenario();
    
    LeArquivoDAT(scene, camera, &height, &width, &box, name);
    LeArquivoPLY(scene, &box, name);
    
    buffer = NULL;
}

RayTracer::~RayTracer() {
    if (buffer) destroy_bitmap(buffer);

    delete camera;
    delete scene;
}
void RayTracer::setCameraPosition(Vetor_3D position) {
    camera->AtribuiPosicao(position);
}

void RayTracer::setCameraDirection(Vetor_3D direction) {
    camera->AtribuiDirecao(direction);
}

void RayTracer::setCameraEye(Vetor_3D eye) {
    camera->AtribuiVUP(eye);
}

void RayTracer::setCamera(Vetor_3D position, Vetor_3D direction, Vetor_3D eye) {
    camera->Atribui(position, direction, eye);
}

bool interceptaBoundingBox(CaixaParalela *box, Raio r_vis) {

	// FAZER PARA CADA BOUNDING BOX
	if (box->Intercepta(r_vis) != -1)
		return true;

	return false;
}

void RayTracer::run() {
    buffer = create_bitmap(width, height);
    
    Raio ray = camera->PrimeiroRaio();
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
			Cor_rgb color;
			if (interceptaBoundingBox(box, ray))
           		color = scene->Intercepta(ray, 0);
			else
				color = Cor_rgb(60, 60, 60);
            putpixel(buffer, j, height-i, RayTracer::makeColor(color));
            ray = camera->ProximoRaio();
        }
    }
}

int RayTracer::makeColor(Cor_rgb color) {
    if (MAXI(color.R(),MAXI(color.G(), color.B())) > 255) printf("AA\n");
    return makecol(color.R() , color.G(), color.B());
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
    int state;
    int width;
    int height;
    
    gcn::Gui *          gui;
    gcn::Container *    top;
    gcn::ImageFont *    font;

    BITMAP *            display;
    VectorInput *       vectorInput;
    RayTracer *         rayTracer;
    gcn::Button *       refresh;

    gcn::AllegroInput *         input;
    gcn::AllegroGraphics *      graphics;
    gcn::AllegroImageLoader *   loader;

private:
    struct VectorListener : public gcn::ActionListener {
        VectorListener(App * const app)
        :   app(app) {}
        
        App * const app;

        void action(const gcn::ActionEvent& actionEvent) {
            if (app->state == 0)
                app->rayTracer->setCameraPosition(app->vectorInput->getVector3D());
            else if (app->state == 1)
                app->rayTracer->setCameraDirection(app->vectorInput->getVector3D());
            else if (app->state == 2)
                app->rayTracer->setCameraEye(app->vectorInput->getVector3D());

            app->rayTracer->run();
        }
    };

    struct RefreshListener : public gcn::ActionListener {
        RefreshListener(App * const app)
        :   app(app) {}
        
        App * const app;
        
        void action(const gcn::ActionEvent& actionEvent) {
            app->rayTracer->run();
        }
    };

    VectorListener * vectorListener;
public:
    App();
    ~App();
    
    void run();
};

App::App() {
    state = 0;

    rayTracer = new RayTracer();
    width = rayTracer->getWidth();
    height = rayTracer->getHeight() + 20;

    allegro_init();
    install_keyboard();
    install_mouse();
    install_timer();
    set_color_depth(16);
    set_alpha_blender();

    set_gfx_mode(GFX_AUTODETECT_WINDOWED, width, height, 0, 0);
    
    display = create_bitmap(width, height);

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
    vectorListener = new VectorListener(this);
    vectorInput->addActionListener(vectorListener);
    top->add(vectorInput, 0, 0);

    refresh = new gcn::Button("Refresh...");
    refresh->setDimension(gcn::Rectangle(0, 0, 50, 20));
    top->add(refresh, width/2, height/2);
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

    allegro_exit();
}

void App::run() {
    rayTracer->run();

    while(!key[KEY_ESC]) {
        gui->logic();
        gui->draw();
        
        blit(rayTracer->getBuffer(), display, 0, 0, 0, 20, width, height-20);
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
