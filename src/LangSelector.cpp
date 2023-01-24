#include "LangSelector.h"

LangSelector::LangSelector() {}
LangSelector::~LangSelector() {}

void ReloadLangs(std::map<std::string, std::string>& langs, std::string translations_base, std::string lang_code)
{
    Translations::LoadTranslation(translations_base, lang_code);

    langs["en"] = Translations::Load("lang_selector.json/lang_en");
    langs["fr"] = Translations::Load("lang_selector.json/lang_fr");
    langs["jp"] = Translations::Load("lang_selector.json/lang_jp");
    langs["ru"] = Translations::Load("lang_selector.json/lang_ru");
    langs["ua"] = Translations::Load("lang_selector.json/lang_ua");
}

void LangSelector::Init(std::string translations_base)
{
    LangSelector::translations_base = translations_base;

    LangSelector::langs = std::map<std::string, std::string>();
    LangSelector::hovered_lang = 0;

    ReloadLangs(LangSelector::langs, LangSelector::translations_base, "en");

    AnimationManager::InitAnim(ANIM_FADE_IN);
    LangSelector::fadeout = false;
}

std::string GetLangCodeByIndex(std::map<std::string, std::string> langs, size_t index)
{
    size_t i = 0;
    for (auto const& lang : langs)
    {
        if (i == index)
            return lang.first;
        
        i++;
    }
    return "";
}

bool LangSelector::Step(Settings* settings, Renderer* rr, Controls ctrl, Controls old_ctrl)
{
    if (settings->Get("language").asString() != "0" && !LangSelector::fadeout)
    {
        Translations::LoadTranslation(LangSelector::translations_base, settings->Get("language").asString());
        return false;
    }

    if (AnimationManager::StepAnim(ANIM_FADE))
        return true;
    else if (fadeout)
        return false;

    if (ctrl.MenuUp() && !old_ctrl.MenuUp())
    {
        rr->GetSounds()->PlaySfx("menu_switch");
        LangSelector::hovered_lang = LangSelector::hovered_lang == 0
                                    ? LangSelector::langs.size() - 1
                                    : LangSelector::hovered_lang - 1;
        
        ReloadLangs(
            LangSelector::langs,
            LangSelector::translations_base,
            GetLangCodeByIndex(LangSelector::langs, LangSelector::hovered_lang)
        );
    }
    if (ctrl.MenuDown() && !old_ctrl.MenuDown())
    {
        rr->GetSounds()->PlaySfx("menu_switch");
        LangSelector::hovered_lang = LangSelector::hovered_lang == LangSelector::langs.size() - 1
                                    ? 0
                                    : LangSelector::hovered_lang + 1;
        
        ReloadLangs(
            LangSelector::langs,
            LangSelector::translations_base,
            GetLangCodeByIndex(LangSelector::langs, LangSelector::hovered_lang)
        );
    }
    if (ctrl.MenuEnter() && !old_ctrl.MenuEnter())
    {
        rr->GetSounds()->PlaySfx("menu_enter");
        LangSelector::fadeout = true;
        AnimationManager::InitAnim(ANIM_FADE_OUT);
        
        settings->Set("language", Json::Value(GetLangCodeByIndex(LangSelector::langs, LangSelector::hovered_lang)));
    }
        
    return true;
}

void LangSelector::Render(Renderer* rr)
{
    bool hover_blinker = LangSelector::fadeout == true && (int)(rr->GetFrames() / 10) % 2;

    int langSelectScale = rr->GetWindowParams().height / 200;
    SDL_Rect textDimensions = rr->GetFont()->GetTextDimensions("-", langSelectScale);
    
    int menu_h = (LangSelector::langs.size()) * textDimensions.h;
    int y_offset = (rr->GetWindowParams().height / 2) - (menu_h / 2);

    SDL_SetRenderDrawColor(rr->GetRenderer(), 0, 0, 0, 0);
    SDL_RenderClear(rr->GetRenderer());

    size_t i = 0;
    for (auto const& lang : LangSelector::langs)
    {
        if (i == LangSelector::hovered_lang && !hover_blinker)
        {
            SDL_Rect hover_bg = rr->GetFont()->GetTextDimensions(lang.second.c_str(), langSelectScale);

            hover_bg.x = (rr->GetWindowParams().width / 2) - (hover_bg.w / 2) - textDimensions.w / 16;
            hover_bg.y = y_offset + textDimensions.h * (int)i - (hover_bg.h / 2) - textDimensions.h / 16;
            hover_bg.w += textDimensions.w / 8;
            hover_bg.h += textDimensions.h / 8;

            SDL_SetRenderDrawColor(rr->GetRenderer(), 0xFF, 0xFF, 0xFF, 0xFF);
            SDL_RenderFillRect(rr->GetRenderer(), &hover_bg);

            rr->RenderText(lang.second.c_str(), rr->GetWindowParams().width / 2, y_offset + textDimensions.h * i, langSelectScale, true, Translations::GetJp(), 0, 0, 0);
        }
        else
            rr->RenderText(lang.second.c_str(), rr->GetWindowParams().width / 2, y_offset + textDimensions.h * i, langSelectScale, true, Translations::GetJp());
        
        i++;
    }
    
    AnimationManager::RenderAnim(ANIM_FADE, rr);
}
