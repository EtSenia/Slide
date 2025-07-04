#include "antlr4-runtime.h"
#include "SlideBaseVisitor.h"
#include "SlideLexer.h"
#include "SlideParser.h"

#include <cmath>
#include <map>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;
using namespace antlr4;
using namespace std;

class SlideDriver : public SlideBaseVisitor
{
    string slideOutput;
    std::vector<std::string> filters;
    int slideCount = 0;

public:
    void saveFile(const string &filePath, const string &content)
    {
        if (!fs::exists("Website")) {
            fs::create_directories("Website");
        }

        std::string fullPath = "Website/" + filePath;
        ofstream outFile(fullPath);
        if (outFile.is_open())
        {
            outFile << content;
            outFile.close();
        }
        else
        {
            std::cerr << "Error al abrir el archivo " + fullPath + " para escribir. \n";
        }
    }

    std::any visitProg(SlideParser::ProgContext *ctx) override
    {
        slideOutput = "";

        for (auto stmt : ctx->stat())
        {
            visit(stmt);
        }

        string htmlOutput = generateHtml();
        string cssOutput = generateCSS();
        string jsOutput = generateJS();

        saveFile("index.html", htmlOutput);
        saveFile("style.css", cssOutput);
        saveFile("script.js", jsOutput);

        std::cout << "Archivos generados con éxito.\n";
        return nullptr;
    }

    std::any visitStat(SlideParser::StatContext *ctx) override
    {
        return visit(ctx->expr());
    }

    std::any visitExpr(SlideParser::ExprContext *ctx) override
    {
        return visit(ctx->slide());
    }

    std::any visitSlide(SlideParser::SlideContext *ctx) override
    {
        string slideId = "slide" + to_string(slideCount++);
        string bgcolor;
        for (auto *background : ctx->background())
        {
            if (background)
            {
                for(auto *bg : background->bg()){
                    if(bg->HEX())
                    {
                        bgcolor = "style=\"background-color:" + bg->HEX()->getText() + "\"";
                    } else if (bg->STR()){
                        string url = bg->STR()->getText();
                        url = url.substr(1, url.size() - 2);
                        bgcolor = "style=\"background-image: url('" + url + "'); background-size: cover;\"";
                    }
                }
            }
        }
        slideOutput += "        <div class=\"slide\"" + bgcolor + " id=\"" + slideId + "\">\n";
        for (auto *el : ctx->children)
        {
            visit(el);
        }
        slideOutput += "        </div>\n";
        return nullptr;
    }

    std::any visitText(SlideParser::TextContext *ctx) override
    {
        slideOutput += "            <p class=\"text\" style=\"";
        for (auto *child : ctx->children)
        {
            visit(child);
        }

        string content;
        string result = "\n";
        for (auto *textProp : ctx->text_prop())
        {
            if (textProp && textProp->textCont())
            {
                if(textProp->textCont()->TEXT_BLOCK()) {
                    content = textProp->textCont()->TEXT_BLOCK()->getText();                    
                    content = content.substr(3, content.size() - 6);
                } else if (textProp->textCont()->STR())
                {
                    content = textProp->textCont()->STR()->getText(); 
                    content = content.substr(1, content.size() - 2);
                }
                content.erase(remove(content.begin(), content.end(), '\t'), content.end());
                size_t pos = 0;
                size_t prevPos = 0;
                bool text_end = false;
                while ((pos = content.find("\n", pos)) != string::npos)
                {
                    text_end = pos != content.length() - 1;
                    result += content.substr(prevPos, pos - prevPos - 1);
                    if (text_end)
                    {
                        result += "<br>";
                    }
                    prevPos = pos + 1;
                    pos += 1;
                }
                result += content.substr(prevPos) + '\n';
            }
        }
        slideOutput += "\">" + result + "            </p>\n";
        return nullptr;
    }

    std::any visitImg(SlideParser::ImgContext *ctx) override
    {
        slideOutput += "            <img style=\"position:absolute;";
        filters.clear();
        for (auto *child : ctx->children)
        {
            visit(child);
        }
        if (!filters.empty()){
            slideOutput += " filter:";
            for (auto f : filters)
            {
                slideOutput +=  f + " ";
            }
        }
            slideOutput += "\">\n";
        return nullptr;
    }

    std::any visitBackground(SlideParser::BackgroundContext *ctx) override
    {
        return nullptr;
    }

    std::any visitTextCont(SlideParser::TextContContext *ctx) override
    {
        return nullptr;
    }

    std::any visitBlur(SlideParser::BlurContext *ctx) override {
        filters.push_back(ctx->getText());
        return visitChildren(ctx);
    }

    std::any visitBrightness(SlideParser::BrightnessContext *ctx) override {
        filters.push_back(ctx->getText());
        return visitChildren(ctx);
    }

    std::any visitContrast(SlideParser::ContrastContext *ctx) override {
        filters.push_back(ctx->getText());
        return visitChildren(ctx);
    }

    std::any visitGrayscale(SlideParser::GrayscaleContext *ctx) override {
        filters.push_back(ctx->getText());
        return visitChildren(ctx);
    }

    std::any visitHue_rotate(SlideParser::Hue_rotateContext *ctx) override {
        filters.push_back(ctx->getText());
        return visitChildren(ctx);
    }

    std::any visitInvert(SlideParser::InvertContext *ctx) override {
        filters.push_back(ctx->getText());
        return visitChildren(ctx);
    }

    std::any visitSaturate(SlideParser::SaturateContext *ctx) override {
        filters.push_back(ctx->getText());
        return visitChildren(ctx);
    }

    std::any visitOpacity(SlideParser::OpacityContext *ctx) override {
        filters.push_back(ctx->getText());
        return visitChildren(ctx);
    }

    std::any visitFilter(SlideParser::FilterContext *ctx) override {
        return visitChildren(ctx);
    }
    
    std::any visitFontSize(SlideParser::FontSizeContext *ctx) override
    {
        slideOutput += "font-size:" + ctx->INT()->getText() + "px;";
        return nullptr;
    }

    std::any visitFontFam(SlideParser::FontFamContext *ctx) override
    {
        string val = ctx->STR()->getText();
        slideOutput += "font-family:" + val.substr(1, val.size() - 2) + ";";
        return nullptr;
    }

    std::any visitAlign(SlideParser::AlignContext *ctx) override
    {
        slideOutput += "text-align:" + ctx->ALIGN()->getText() + ";";
        return nullptr;
    }

    std::any visitSrc(SlideParser::SrcContext *ctx) override
    {
        string src = ctx->STR()->getText();
        src = src.substr(1, src.size() - 2);
        slideOutput += "content:url('" + src + "');";
        return nullptr;
    }

    std::any visitPosition(SlideParser::PositionContext *ctx) override
    {
        auto unidades = ctx->UNIDAD();
        if (unidades.size() == 2)
        {
            slideOutput += "left:" + unidades[0]->getText() + ";top:" + unidades[1]->getText() + ";";
        }
        return nullptr;
    }

    std::any visitWidth(SlideParser::WidthContext *ctx) override
    {
        slideOutput += "width:" + ctx->UNIDAD()->getText() + ";";
        return nullptr;
    }

    std::any visitHeight(SlideParser::HeightContext *ctx) override
    {
        slideOutput += "height:" + ctx->UNIDAD()->getText() + ";";
        return nullptr;
    }

    std::any visitTextColor(SlideParser::TextColorContext *ctx) override {
        slideOutput += "color:" + ctx->HEX()->getText() + ";";
        return nullptr;
    }

    std::string generateHtml()
    {
        return R"delimeter(<!DOCTYPE html>
<html lang="es">
<head>
    <meta charset="UTF-8" />
    <meta name="viewport" content="width=device-width, initial-scale=1.0" />
    <title>Presentación Animada</title>
    <link rel="stylesheet" href="style.css" />
</head>
<body>
    <div class="slider">
        <button id="prev" class="nav-button" onclick="prevSlide()"><</button>
        <button id="next" class="nav-button" onclick="nextSlide()">></button>
)delimeter" + slideOutput +
               R"delimeter(
    </div>
        
    <div class="controls">
        <button onclick="prevSlide()">⟨ Anterior</button>
        <button id="toggle-mode" class="nav-button" onclick="toggleFullscreen()">Presentar</button>
        <button onclick="nextSlide()">Siguiente ⟩</button>
    </div>

    <script src="script.js"></script>
</body>
</html>)delimeter";
    }

    std::string generateCSS()
    {
        return R"(* {
    margin: 0;
    padding: 0;
    box-sizing: border-box;
}

body,
html {
    width: 100%;
    height: 100%;
    font-family: sans-serif;
    overflow: hidden;
    background: #111;
    justify-content: center;
    align-items: center;
    margin: auto;
}

body {
    display: flex;
    flex-direction: column;
}

.slider {
    position: relative;
    aspect-ratio: 16 / 9;
    justify-content: center;
    overflow: hidden;
}

.slide {
    position: absolute;
    width: 1920px;
    aspect-ratio: 16 / 9;
    padding: 2rem;
    opacity: 0;
    transform-origin: top left;
    transition: all 0.6s ease;
    background: #fff;
    display: flex;
    flex-direction: column;
    justify-content: center;
    align-items: center;
    overflow: hidden;
}

.slide.active {
    left: 0;
    opacity: 1;
    z-index: 2;
}

.slide.exit-left {
    left: -100%;
    opacity: 0;
}

.slide.exit-right {
    left: 100%;
    opacity: 0;
}

.controls {
    text-align: center;
    padding: 1rem;
}

button {
    background: #444;
    color: white;
    border: none;
    padding: 0.7rem 1.2rem;
    margin: 0 1rem;
    cursor: pointer;
    font-size: 1rem;
    border-radius: 5px;
    transition: background 0.3s;
}

button:hover {
    background: #666;
}

.text {
    position: absolute;
    transform: translate(-50%, -50%);
}
img {    
    transform: translate(-50%, -50%);
}

#prev,
#next {
    position: absolute;
    top: 0;
    height: 100%;
    width: 20%;
    opacity: 0;
    margin: 0;
    padding: 0;
    color: white;
    background: transparent;
    border: none;
    cursor: pointer;
    font-size: 1.2rem;
    z-index: 10;
}

#prev {
    left: 0px;
    background-image: linear-gradient(
        to left,
        rgba(0, 0, 0, 0),
        rgba(0, 0, 0, 0.5)
);
}

#next {
    right: 0px;
    background-image: linear-gradient(
        to right,
        rgba(0, 0, 0, 0),
        rgba(0, 0, 0, 0.5)
);
}

#prev:hover,
#next:hover {
    opacity: 1;
}

)";
    }

    std::string generateJS()
    {
        return R"delimeter(const slides = document.querySelectorAll(".slide");
const slider = document.querySelector(".slider");
const controls = document.querySelector(".controls");

let current = 0;

function showSlide(index, direction) {
    const currentSlide = slides[current];
    currentSlide.classList.remove("active");

    if (direction === "next") {
        currentSlide.classList.add("exit-left");
    } else {
        currentSlide.classList.add("exit-right");
    }

    setTimeout(() => {
        currentSlide.classList.remove("exit-left", "exit-right");
    }, 600);

    current = index;
    const nextSlide = slides[current];
    nextSlide.classList.add("active");
}

function nextSlide() {
    if (current === slides.length - 1) {
        return;
    }
    const next = (current + 1) % slides.length;
    showSlide(next, "next");
    }

    function prevSlide() {
    if (current === 0) {
        return;
    }
    const prev = (current - 1 + slides.length) % slides.length;
    showSlide(prev, "prev");
}

document.addEventListener("keydown", (event) => {
    if (event.key === "ArrowRight") {
        nextSlide();
    } else if (event.key === "ArrowLeft") {
        prevSlide();
    }
});

function adjustSlideSize() {
    const vw = window.innerWidth;
    const vh = window.innerHeight;
    let maxWidth = vw * 0.9;
    let maxHeight = vh - 75;
    if (document.fullscreenElement) {
        maxWidth = document.fullscreenElement.clientWidth;
        maxHeight = document.fullscreenElement.clientHeight;
    }

    const newScaleW = maxWidth / 1920;
    const newScaleH = maxHeight / 1080;
    const newScale = Math.min(newScaleW, newScaleH);

    let width = newScaleW > newScaleH ? maxHeight * 16 / 9 : maxWidth;
    slider.style.width = `${width}px`;
    for (let i = 0; i < slides.length; i++) {
        slides[i].style.transform = `scale(${newScale})`;
    }
}

function checkFullscreen() {
    if (document.fullscreenElement) {
        controls.style.display = "none";
    } else {
        controls.style.display = "block";
        adjustSlideSize();
    }
}

window.addEventListener("fullscreenchange", checkFullscreen);

function toggleFullscreen() {
    if (!document.fullscreenElement) {
        document.documentElement.requestFullscreen().catch((err) => {
        console.log("Error al intentar activar pantalla completa: " + err);
        });
    } else {
        if (document.exitFullscreen) {
        document.exitFullscreen().catch((err) => {
            console.log("Error al intentar salir de pantalla completa: " + err);
        });
        }
    }
}

window.addEventListener("resize", adjustSlideSize);
adjustSlideSize();
slides[0].classList.add("active");
)delimeter";
    }
};