#include "LangSelector.h"

LangSelector::LangSelector() {}
LangSelector::~LangSelector() {}

std::string LangSelector::GetLangCodeByIndex(size_t index)
{
    size_t i = 0;
    for (auto const& lang : LangSelector::langs)
    {
        if (i++ == index)
            return lang.first;
    }
    return "";
}

void LangSelector::ReloadLang(std::string lang_code)
{
    Translations::LoadTranslation(LangSelector::translations_base, lang_code);

    LangSelector::langs["en"] = Translations::Load("lang_selector.json/lang_en");
    LangSelector::langs["fr"] = Translations::Load("lang_selector.json/lang_fr");
    LangSelector::langs["jp"] = Translations::Load("lang_selector.json/lang_jp");
    LangSelector::langs["ru"] = Translations::Load("lang_selector.json/lang_ru");
    LangSelector::langs["ua"] = Translations::Load("lang_selector.json/lang_ua");

    LangSelector::choose_title = Translations::Load("lang_selector.json/choose_title");
    LangSelector::settings_reminder = Translations::Load("lang_selector.json/settings_reminder");
}

void LangSelector::Init(std::string translations_base)
{
    LangSelector::translations_base = translations_base;

    LangSelector::langs = std::map<std::string, std::string>();
    LangSelector::hovered_lang = 0;

    LangSelector::ReloadLang("en");

    AnimationManager::InitAnim(ANIM_FADE_IN);
    LangSelector::fadeout = false;
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
    
    if (ctrl.MenuEnter())
    {
        rr->GetSounds()->PlaySfx("menu_enter");
        LangSelector::fadeout = true;
        AnimationManager::InitAnim(ANIM_FADE_OUT);
        
        settings->Set("language", Json::Value(GetLangCodeByIndex(LangSelector::hovered_lang)));

        return true;
    }

    if (rr->GetCursor(old_ctrl))
    {
        int langSelectScale = rr->GetHeight() / 200;
        SDL_Rect textDimensions = rr->GetFont()->GetTextDimensions("-", langSelectScale);

        float distanceScale = 1.2;
        
        int menu_h = (LangSelector::langs.size()) * textDimensions.h * distanceScale;
        int y_offset = (rr->GetHeight() / 2) - (menu_h / 2);

        size_t i = 0;
        for (auto const& lang : LangSelector::langs)
        {
            SDL_Rect hover = rr->GetFont()->GetTextDimensions(lang.second.c_str(), langSelectScale);

            hover.x = (rr->GetWidth() / 2) - (hover.w / 2) - textDimensions.w / 16;
            hover.y = y_offset + textDimensions.h * distanceScale * (int)i - (hover.h / 2) - textDimensions.h / 16;
            hover.w += textDimensions.w / 8;
            hover.h += textDimensions.h / 8;

            SDL_Point temp = ctrl.GetMouse();
            if (SDL_PointInRect(&temp, &hover))
            {
                if (i != LangSelector::hovered_lang)
                {
                    rr->GetSounds()->PlaySfx("menu_switch");
                    LangSelector::hovered_lang = i;

                    LangSelector::ReloadLang(LangSelector::GetLangCodeByIndex(LangSelector::hovered_lang));
                }
                if (old_ctrl.MenuMouse() && !ctrl.MenuMouse())
                {
                    rr->GetSounds()->PlaySfx("menu_enter");
                    LangSelector::fadeout = true;
                    AnimationManager::InitAnim(ANIM_FADE_OUT);
                    
                    settings->Set("language", Json::Value(GetLangCodeByIndex(LangSelector::hovered_lang)));
                }
                return true;
            }

            i++;
        }
    }

    if (ctrl.MenuUp() && !old_ctrl.MenuUp())
    {
        rr->GetSounds()->PlaySfx("menu_switch");
        LangSelector::hovered_lang = LangSelector::hovered_lang == 0
                                    ? LangSelector::langs.size() - 1
                                    : LangSelector::hovered_lang - 1;
        
        LangSelector::ReloadLang(LangSelector::GetLangCodeByIndex(LangSelector::hovered_lang));
    }
    if (ctrl.MenuDown() && !old_ctrl.MenuDown())
    {
        rr->GetSounds()->PlaySfx("menu_switch");
        LangSelector::hovered_lang = LangSelector::hovered_lang == LangSelector::langs.size() - 1
                                    ? 0
                                    : LangSelector::hovered_lang + 1;
        
        LangSelector::ReloadLang(LangSelector::GetLangCodeByIndex(LangSelector::hovered_lang));
    }
        
    return true;
}

void LangSelector::Render(Renderer* rr)
{
    bool hover_blinker = LangSelector::fadeout == true && (int)(rr->GetFrames() / 10) % 2;

    int langSelectScale = rr->GetHeight() / 200;
    SDL_Rect textDimensions = rr->GetFont()->GetTextDimensions("-", langSelectScale);

    float distanceScale = 1.2;
    
    int menu_h = (LangSelector::langs.size()) * textDimensions.h * distanceScale;
    int y_offset = (rr->GetHeight() / 2) - (menu_h / 2);

    SDL_SetRenderDrawColor(rr->GetRenderer(), 0x10, 0x10, 0x10, 0);
    SDL_RenderClear(rr->GetRenderer());

    size_t i = 0;
    for (auto const& lang : LangSelector::langs)
    {
        if (i == LangSelector::hovered_lang)
        {
            SDL_Rect hover_bg = rr->GetFont()->GetTextDimensions(lang.second.c_str(), langSelectScale);

            hover_bg.x = (rr->GetWidth() / 2) - (hover_bg.w / 2) - textDimensions.w / 16;
            hover_bg.y = y_offset + textDimensions.h * distanceScale * (int)i - (hover_bg.h / 2) - textDimensions.h / 16;
            hover_bg.w += textDimensions.w / 8;
            hover_bg.h += textDimensions.h / 8;

            SDL_SetRenderDrawColor(rr->GetRenderer(), 0xFF, 0xFF, 0xFF, 0xFF);
            SDL_RenderFillRect(rr->GetRenderer(), &hover_bg);
            
            rr->RenderText(
                "▶",
                rr->GetWidth() / 2 - rr->GetFont()->GetTextDimensions(lang.second.c_str(), langSelectScale).w / 2 - textDimensions.w,
                y_offset + textDimensions.h * distanceScale * i,
                langSelectScale,
                true,
                true
            );

            if (!hover_blinker)
                rr->RenderText(
                    lang.second.c_str(),
                    rr->GetWidth() / 2,
                    y_offset + textDimensions.h * distanceScale * i,
                    langSelectScale,
                    true,
                    Translations::GetJp(), 0, 0, 0
                );
        }
        else
            rr->RenderText(
                lang.second.c_str(),
                rr->GetWidth() / 2,
                y_offset + textDimensions.h * distanceScale * i,
                langSelectScale,
                true,
                Translations::GetJp()
            );
        
        i++;
    }

    rr->RenderText(
        LangSelector::choose_title.c_str(),
        rr->GetWidth() / 2,
        rr->GetHeight() / 16,
        Translations::GetJp() ? langSelectScale : langSelectScale / 2,
        true, Translations::GetJp()
    );
    rr->RenderText(
        LangSelector::settings_reminder.c_str(),
        rr->GetWidth() / 2,
        (rr->GetHeight() / 16) * 15,
        Translations::GetJp() ? langSelectScale : langSelectScale / 2,
        true, Translations::GetJp()
    );
    
    AnimationManager::RenderAnim(ANIM_FADE, rr);
}
