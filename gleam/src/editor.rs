//! A simple example that demonstrates using conrod within a basic `winit` window loop, using
//! `glium` to render the `conrod_core::render::Primitives` to screen.

use crate::support;

use std::fs::File;
use std::fs::OpenOptions;
use std::io::Read;
use std::io::Write;

use conrod_core::{widget, Colorable, Positionable, Widget};
use glium::Surface;

use rfd::FileDialog;

const WIDTH: u32 = 400;
const HEIGHT: u32 = 200;
widget_ids! {
    struct Ids { canvas, text_edit, scrollbar }
}

pub fn edit_file(file_name: &str) {
    const WIDTH: u32 = 360;
    const HEIGHT: u32 = 720;

    // Build the window.
    let event_loop = glium::glutin::event_loop::EventLoop::new();
    let window = glium::glutin::window::WindowBuilder::new()
        .with_title("TextEdit Demo")
        .with_inner_size(glium::glutin::dpi::LogicalSize::new(WIDTH, HEIGHT));
    let context = glium::glutin::ContextBuilder::new()
        .with_vsync(true)
        .with_multisampling(4);
    let display = glium::Display::new(window, context, &event_loop).unwrap();

    // Construct our `Ui`.
    let mut ui = conrod_core::UiBuilder::new([WIDTH as f64, HEIGHT as f64]).build();

    // A unique identifier for each widget.
    let ids = Ids::new(ui.widget_id_generator());

    // Add a `Font` to the `Ui`'s `font::Map` from file.
    let assets = find_folder::Search::KidsThenParents(3, 5)
        .for_folder("assets")
        .unwrap();
    let font_path = assets.join("Cascadia.ttf");
    ui.fonts.insert_from_file(font_path).unwrap();

    // A type used for converting `conrod_core::render::Primitives` into `Command`s that can be used
    // for drawing to the glium `Surface`.
    let mut renderer = conrod_glium::Renderer::new(&display).unwrap();

    // The image map describing each of our widget->image mappings (in our case, none).
    let image_map = conrod_core::image::Map::<glium::texture::Texture2d>::new();

    // Some starting text to edit.
    let text = read_file(file_name);
    let mut demo_text = text.to_owned();
    let file_path = file_name.to_owned();

    let mut ctrl_down = false;

    // Poll events from the window.
    support::run_loop(display, event_loop, move |request, display| {
        match request {
            support::Request::Event {
                event,
                should_update_ui,
                should_exit,
            } => {
                // Use the `winit` backend feature to convert the winit event to a conrod one.
                if let Some(event) = support::convert_event(&event, &display.gl_window().window()) {
                    ui.handle_event(event);
                    *should_update_ui = true;
                }

                match event {
                    glium::glutin::event::Event::WindowEvent { event, .. } => match event {
                        // Break from the loop upon `Escape`.
                        glium::glutin::event::WindowEvent::CloseRequested => *should_exit = true,
                        glium::glutin::event::WindowEvent::ModifiersChanged(state) => ctrl_down = state.ctrl(),

                        // Save event handler.
                        glium::glutin::event::WindowEvent::KeyboardInput {
                            input:
                                glium::glutin::event::KeyboardInput {
                                    virtual_keycode:
                                        Some(glium::glutin::event::VirtualKeyCode::S),
                                    ..
                                },
                            ..
                        } => {
                            if ctrl_down {
                                save_file(&file_path, &demo_text);
                                ctrl_down = false;
                            }
                        },
                        _ => {}
                    },
                    _ => {}
                }
            }
            support::Request::SetUi { needs_redraw } => {
                // Instantiate all widgets in the GUI.
                set_ui(ui.set_widgets(), &ids, &mut demo_text);

                // Get the underlying winit window and update the mouse cursor as set by conrod.
                display
                    .gl_window()
                    .window()
                    .set_cursor_icon(support::convert_mouse_cursor(ui.mouse_cursor()));

                *needs_redraw = ui.has_changed();
            }
            support::Request::Redraw => {
                // Render the `Ui` and then display it on the screen.
                let primitives = ui.draw();

                renderer.fill(display, primitives, &image_map);
                let mut target = display.draw();
                target.clear_color(0.0, 0.0, 0.0, 1.0);
                renderer.draw(display, &mut target, &image_map).unwrap();
                target.finish().unwrap();
            }
        }
    })
}

// Declare the `WidgetId`s and instantiate the widgets.
fn set_ui(ref mut ui: conrod_core::UiCell, ids: &Ids, demo_text: &mut String) {
    use conrod_core::{color, widget, Colorable, Positionable, Sizeable, Widget};

    widget::Canvas::new()
        .scroll_kids_vertically()
        .color(color::DARK_CHARCOAL)
        .set(ids.canvas, ui);

    for edit in widget::TextEdit::new(demo_text)
        .color(color::WHITE)
        .padded_w_of(ids.canvas, 20.0)
        .mid_top_of(ids.canvas)
        .left_justify()
        .line_spacing(10.5)
        .restrict_to_height(false) // Let the height grow infinitely and scroll.
        .set(ids.text_edit, ui)
    {
        *demo_text = edit;
    }

    widget::Scrollbar::y_axis(ids.canvas)
        .auto_hide(true)
        .set(ids.scrollbar, ui);
}

fn read_file(file_name: &str) -> String {
    // TODO: error handling.
    match File::open(file_name) {
        Ok (mut reader) => {
            let mut file_content = String::new();
            reader.read_to_string(&mut file_content).unwrap();
            return file_content;
        }
        _ => String::new()
    }
}

fn save_file(file_name: &str, text: &str) {
    // TODO: error handling.
    let mut writer = OpenOptions::new()
        .create(true)
        .write(true)
        .open(file_name)
        .unwrap();

    writer.write_all(text.as_bytes()).unwrap();
}