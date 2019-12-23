#include "gtest/gtest.h"

#include "../../Mocks/MockGraphicsContext.h"
#include "../../Mocks/MockViewport.h"
#include "../../../Renderer/TextBox/TextBox.h"

TEST(TextBoxTests, EmptyTextBox)
{

    std::shared_ptr<TextDocument> textDocument(new TextDocument);

    std::shared_ptr<TextBox> textBox(
        new TextBox(
            textDocument,
            colors::Black,
            colors::White,
            0,
            0,
            100,
            100));

    std::shared_ptr<MockGraphicsContext> mockGraphicsContext(new MockGraphicsContext);

    MockViewport mockViewport(100, 100);
    mockViewport.GetChildren().push_back(textBox);

    mockViewport.Render(mockGraphicsContext);
}
