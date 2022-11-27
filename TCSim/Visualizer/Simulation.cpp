#include "Simulation.hpp"
// Simulation Engine

void Simulation::initVariables()
{
    this->window = nullptr;
}

void Simulation::initWindow()
{
    this->videoMode.height = 680;
    this->videoMode.width = 1080;
    this->window = new sf::RenderWindow(this->videoMode, "City Simulator", sf::Style::Titlebar | sf::Style::Close);
    this->window->setFramerateLimit(60);
}

void Simulation::initObjects()
{

    // load road objects for drawing

    std::vector<infoRoad> aux = this->city.get_roads();
    std::vector<infoNode> aux2 = this->city.get_info_nodes();

    this->cityAux.initCity(aux, aux2);
    this->addCar.init(890.f, 80.f, 140.f, 35.f, "Add 1 Car");
    this->removeCar.init(890.f, 135.f, 140.f, 35.f, "Remove 1 Car");
    this->Quit.init(890.f, 190.f, 70.f, 35.f, "Quit");
    this->menu.init();
}

Simulation::Simulation()
{
    this->city = City();
    this->agents = AgentManager(city);
    this->initVariables();
    this->initWindow();
    this->initObjects();
}

Simulation::Simulation(std::string filename)
{
    this->window = nullptr;

    this->city = City(filename);
    this->agents = AgentManager(city);
    this->initWindow();
    this->initObjects();
}

Simulation::~Simulation()
{
    delete this->window;
}

const bool Simulation::running() const
{
    return this->window->isOpen();
}

void Simulation::pollEvents()
{
    while (this->window->pollEvent(this->ev))
    {
        switch (this->ev.type)
        {
        // case sf::Event::KeyPressed:
        // if (this->ev.key.code == sf::Keyboard::Up)
        // break;
        case sf::Event::Closed:
            // send close window event
            window->close();
            break;
        case sf::Event::MouseButtonPressed:
        case sf::Event::MouseMoved:
            this->addCar.update(sf::Mouse::getPosition(*this->window));
            this->removeCar.update(sf::Mouse::getPosition(*this->window));
            this->Quit.update(sf::Mouse::getPosition(*this->window));
            break;

        case sf::Event::MouseButtonReleased:
            if (this->addCar.isPressed())
            {
                Node *n = this->city.get_randomNode();
                this->agents.newCar(*n, 20);
            }

            else if (this->removeCar.isPressed())
                this->agents.deleteCar();

            else if (this->Quit.isPressed())
            {
                // this->city.printAll();
                this->window->close();
            }
            break;
        }
    }
}

void Simulation::update()
{
    // std::cout << "Mouse pos: " << sf::Mouse::getPosition().x << " ";
    // std::cout << sf::Mouse::getPosition().y << "\n";
    // std::cout << "Mouse pos: " << sf::Mouse::getPosition(*this->window).x << " ";
    // std::cout << sf::Mouse::getPosition(*this->window).y << "\n";
    this->pollEvents();
    this->agents.update();
}

void Simulation::render()
{
    /*
        - clear old frames
        - render objects
        - display frame in window
    */
    this->window->clear(sf::Color{100, 200, 100, 255});

    // draw U.I
    this->menu.draw(window);
    this->addCar.draw(window);
    this->removeCar.draw(window);
    this->Quit.draw(window);

    // draw city & cars
    this->cityAux.draw(window);
    this->agents.draw(window);

    this->window->display();
    sf::Clock clock;
    // while (clock.getElapsedTime() <= sf::seconds(1.f)) {};
}
