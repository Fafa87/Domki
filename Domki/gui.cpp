#include "gui.h"
#include "windows.h"

GUI* GUI::apa;

void GUI::setup_theme()
{
    GUI::aplikacja().sfgui.AddCharacterSet(0x0000, 0x017D);
    pulpit.LoadThemeFromFile("Grafika\\bazowy.theme");
}

void GUI::center_window(shared_ptr<sfg::Window> window)
{
    auto win_rect = window->GetAllocation();
    sf::Vector2f size(win_rect.width, win_rect.height);
    auto parent_size = (sf::Vector2f)okno.getSize();
    window->SetPosition((parent_size - size) / 2.0f);
}

void GUI::bottom_window(shared_ptr<sfg::Window> window)
{
    auto win_rect = window->GetAllocation();
    sf::Vector2f size(win_rect.width, win_rect.height);
    auto final_position = (sf::Vector2f)okno.getSize();
    final_position.y -= size.y;
    final_position.x = (final_position.x - size.x) / 2;
    window->SetPosition(final_position);
}

void GUI::bottom_left_window(shared_ptr<sfg::Window> window)
{
    auto win_rect = window->GetAllocation();
    sf::Vector2f size(win_rect.width, win_rect.height);
    auto final_position = (sf::Vector2f)okno.getSize();
    final_position.y -= size.y;
    final_position.x = 0;
    window->SetPosition(final_position);
}

void GUI::put_right_to(shared_ptr<sfg::Window> window, int startFromX)
{
    auto bottom_right = okno.mapCoordsToPixel(sf::Vector2f(startFromX, 0));
    auto window_width = okno.getSize().x - bottom_right.x;

    window->SetAllocation(sf::FloatRect(bottom_right.x, 0, window_width, window->GetAllocation().height));
}

void GUI::top_right_window(shared_ptr<sfg::Window> window)
{
    auto win_rect = window->GetAllocation();
    sf::Vector2f size(win_rect.width, win_rect.height);
    auto final_position = (sf::Vector2f)okno.getSize();
    final_position.y = 0;
    final_position.x -= size.x;
    window->SetPosition(final_position);
}

void GUI::stretch_up_down(shared_ptr<sfg::Window> window)
{
    auto win_rect = window->GetAllocation();
    window->SetRequisition(sf::Vector2f(win_rect.width, okno.getSize().y));
}

void GUI::wait_for_anything()
{
    sf::Event event;
    while (true)
    {
        okno.pollEvent(event);
        if (event.type == sf::Event::KeyReleased || event.type == sf::Event::MouseButtonReleased)
            break;

        GUI::aplikacja().pulpit.HandleEvent(event);
        
        Sleep(100);
    }
}

void GUI::show_and_wait_for_anything(shared_ptr<sfg::Window> window)
{
    set_active_window(window);
    center_window(window);

    pulpit.Update(1);
    render();

    wait_for_anything();

    remove_active_window(window);
}

void GUI::set_active_window(shared_ptr<sfg::Window> window)
{
    if (window == nullptr)
        return;
    pulpit.Add(window);
    window->Show(true);
    windows.push_back(window);
}

void GUI::pop_active_window(shared_ptr<sfg::Window> window)
{
    remove_active_window(window);

    if (windows.size() > 0)
        set_active_window(windows.back());
}

void GUI::remove_active_window(shared_ptr<sfg::Window> window)
{
    pulpit.Remove(window);
    window->Show(false);
    windows.erase(std::remove(windows.begin(), windows.end(), window), windows.end());
}

void GUI::show_all_windows()
{
    for (auto& w : windows)
        w->Show(true);
}

void GUI::hide_all_windows()
{
    for (auto& w : windows)
        w->Show(false);
}

void GUI::render()
{
    pulpit.Update(0.001f);
    GUI::aplikacja().sfgui.Display(okno);
    okno.display();
}

void GUI::reset_view()
{
    okno.setView(zwykly_widok);
    okno.draw(nico);
}

void GUI::commit_properties_batch()
{
    GUI::aplikacja().pulpit.SetProperties(new_theme_string.str());
    new_theme_string.clear();
}

void GUI::set_viewport_abs(sf::View& view, sf::FloatRect rect)
{
    auto size = okno.getSize();
    rect = sf::FloatRect(rect.left, rect.top, rect.width <= 0 ? size.x + rect.width : rect.width, rect.height <= 0 ? size.y + rect.height : rect.height);
    view.setViewport(sf::FloatRect(rect.left / size.x, rect.top / size.y, rect.width / size.x, rect.height / size.y));
}

void GUI::show_bottom_gui(sf::View& view, std::shared_ptr<sfg::Window> gui_okno)
{
    gui_okno->SetRequisition(sf::Vector2f(okno.getSize().x, gui_okno->GetRequisition().y));
    bottom_window(gui_okno);

    view.setViewport(sf::FloatRect(0.0f, 0.0f, 1.0f, 1.0f));
    okno.setView(view);

    okno.draw(nico);
    pulpit.Update(0.001f); // -> 1300 MB
    sfgui.Display(okno);

    set_viewport_abs(view, sf::FloatRect(0.0f, 0.0f, 0.0f, -gui_okno->GetAllocation().height));
    okno.setView(view);
}

void GUI::finish_viewport_render(sf::View & view)
{
    view.setViewport(sf::FloatRect(0.0f, 0.0f, 1.0f, 1.0f));
    okno.setView(view);

    okno.draw(nico);
}
