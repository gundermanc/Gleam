#include "gtest/gtest.h"

#include "../../Mocks/MockGraphicsContext.h"
#include "../../Mocks/MockViewport.h"
#include "../../../Renderer/TextBox/TextBox.h"

namespace
{
    std::tuple<std::shared_ptr<TextBox>, std::shared_ptr<MockViewport>> CreateViewport()
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

        std::shared_ptr<MockViewport> mockViewport(new MockViewport(100, 100));
        mockViewport->GetChildren().push_back(textBox);
        return std::make_tuple(textBox, mockViewport);
    }

    // Verify that the caret is correctly placed and updates position
    // when the user types in an empty document.
    TEST(TextBoxTests, EmptyTextBoxTyping)
    {
        auto textBoxAndViewport = CreateViewport();
        auto textBox = std::get<0>(textBoxAndViewport);
        auto viewPort = std::get<1>(textBoxAndViewport);

        // Initial pass.
        viewPort->Render();

        // Check caret position.
        EXPECT_EQ(0, textBox->GetCaretPosition().GetValue());
        EXPECT_EQ(0, textBox->GetTextDocument().GetCurrentMoment()->GetLineCount());
        EXPECT_STREQ("", textBox->GetTextDocument().ToString().c_str());

        // Type a few characters and validate.
        EXPECT_TRUE(textBox->SendKey(Key::None, KeyAction::TypeCharacter, 'H'));
        EXPECT_EQ(1, textBox->GetTextDocument().GetCurrentMoment()->GetLineCount());
        EXPECT_EQ(1, textBox->GetCaretPosition().GetValue());
        EXPECT_STREQ("H", textBox->GetTextDocument().ToString().c_str());

        EXPECT_TRUE(textBox->SendKey(Key::None, KeyAction::TypeCharacter, 'e'));
        EXPECT_EQ(1, textBox->GetTextDocument().GetCurrentMoment()->GetLineCount());
        EXPECT_EQ(2, textBox->GetCaretPosition().GetValue());
        EXPECT_STREQ("He", textBox->GetTextDocument().ToString().c_str());

        EXPECT_TRUE(textBox->SendKey(Key::None, KeyAction::TypeCharacter, 'l'));
        EXPECT_EQ(1, textBox->GetTextDocument().GetCurrentMoment()->GetLineCount());
        EXPECT_EQ(3, textBox->GetCaretPosition().GetValue());
        EXPECT_STREQ("Hel", textBox->GetTextDocument().ToString().c_str());

        EXPECT_TRUE(textBox->SendKey(Key::None, KeyAction::TypeCharacter, 'l'));
        EXPECT_EQ(1, textBox->GetTextDocument().GetCurrentMoment()->GetLineCount());
        EXPECT_EQ(4, textBox->GetCaretPosition().GetValue());
        EXPECT_STREQ("Hell", textBox->GetTextDocument().ToString().c_str());

        EXPECT_TRUE(textBox->SendKey(Key::None, KeyAction::TypeCharacter, 'o'));
        EXPECT_EQ(1, textBox->GetTextDocument().GetCurrentMoment()->GetLineCount());
        EXPECT_EQ(5, textBox->GetCaretPosition().GetValue());
        EXPECT_STREQ("Hello", textBox->GetTextDocument().ToString().c_str());
    }

    // Verify that the caret is correctly placed and updates position
    // when the user backspaces in a single line document.
    TEST(TextBoxTests, SingleLineBackspace)
    {
        auto textBoxAndViewport = CreateViewport();
        auto textBox = std::get<0>(textBoxAndViewport);
        auto viewPort = std::get<1>(textBoxAndViewport);

        // Set text + caret position.
        textBox->GetTextDocument().Insert("Hello", 0);
        textBox->GetCaretPosition().SetValue(5);

        // Initial pass.
        viewPort->Render();

        // Backspace and validate.
        EXPECT_TRUE(textBox->SendKey(Key::Backspace, KeyAction::Press, '\0'));
        EXPECT_EQ(1, textBox->GetTextDocument().GetCurrentMoment()->GetLineCount());
        EXPECT_EQ(4, textBox->GetCaretPosition().GetValue());
        EXPECT_STREQ("Hell", textBox->GetTextDocument().ToString().c_str());

        EXPECT_TRUE(textBox->SendKey(Key::Backspace, KeyAction::Press, '\0'));
        EXPECT_EQ(1, textBox->GetTextDocument().GetCurrentMoment()->GetLineCount());
        EXPECT_EQ(3, textBox->GetCaretPosition().GetValue());
        EXPECT_STREQ("Hel", textBox->GetTextDocument().ToString().c_str());

        EXPECT_TRUE(textBox->SendKey(Key::Backspace, KeyAction::Press, '\0'));
        EXPECT_EQ(1, textBox->GetTextDocument().GetCurrentMoment()->GetLineCount());
        EXPECT_EQ(2, textBox->GetCaretPosition().GetValue());
        EXPECT_STREQ("He", textBox->GetTextDocument().ToString().c_str());

        EXPECT_TRUE(textBox->SendKey(Key::Backspace, KeyAction::Press, '\0'));
        EXPECT_EQ(1, textBox->GetTextDocument().GetCurrentMoment()->GetLineCount());
        EXPECT_EQ(1, textBox->GetCaretPosition().GetValue());
        EXPECT_STREQ("H", textBox->GetTextDocument().ToString().c_str());

        EXPECT_TRUE(textBox->SendKey(Key::Backspace, KeyAction::Press, '\0'));
        EXPECT_EQ(1, textBox->GetTextDocument().GetCurrentMoment()->GetLineCount());
        EXPECT_EQ(0, textBox->GetCaretPosition().GetValue());
        EXPECT_STREQ("", textBox->GetTextDocument().ToString().c_str());

        EXPECT_TRUE(textBox->SendKey(Key::Backspace, KeyAction::Press, '\0'));
        EXPECT_EQ(1, textBox->GetTextDocument().GetCurrentMoment()->GetLineCount());
        EXPECT_EQ(0, textBox->GetCaretPosition().GetValue());
        EXPECT_STREQ("", textBox->GetTextDocument().ToString().c_str());
    }

    // Verify that the caret is correctly placed and updates position
    // when the user types ENTER in a document.
    TEST(TextBoxTests, MultiLineTyping)
    {
        auto textBoxAndViewport = CreateViewport();
        auto textBox = std::get<0>(textBoxAndViewport);
        auto viewPort = std::get<1>(textBoxAndViewport);

        // Initial pass.
        viewPort->Render();

        // Type with some ENTERs and validate.
        EXPECT_TRUE(textBox->SendKey(Key::None, KeyAction::TypeCharacter, 'H'));
        EXPECT_TRUE(textBox->SendKey(Key::None, KeyAction::TypeCharacter, 'i'));
        EXPECT_TRUE(textBox->SendKey(Key::Enter, KeyAction::Press, '\0'));
        EXPECT_EQ(4, textBox->GetCaretPosition().GetValue());
        EXPECT_EQ(1, textBox->GetTextDocument().GetCurrentMoment()->GetLineCount());
        EXPECT_STREQ("Hi\r\n", textBox->GetTextDocument().ToString().c_str());

        EXPECT_TRUE(textBox->SendKey(Key::None, KeyAction::TypeCharacter, 'B'));
        EXPECT_TRUE(textBox->SendKey(Key::None, KeyAction::TypeCharacter, 'y'));
        EXPECT_TRUE(textBox->SendKey(Key::None, KeyAction::TypeCharacter, 'e'));
        EXPECT_TRUE(textBox->SendKey(Key::Enter, KeyAction::Press, '\0'));
        EXPECT_EQ(9, textBox->GetCaretPosition().GetValue());
        EXPECT_EQ(2, textBox->GetTextDocument().GetCurrentMoment()->GetLineCount());
        EXPECT_STREQ("Hi\r\nBye\r\n", textBox->GetTextDocument().ToString().c_str());

        // TODO: Validate lines text.
        //EXPECT_STREQ("Hi")
    }
}