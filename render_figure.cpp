#include "weblegends.h"
#include "helpers.h"

#include "modules/Translation.h"

#include "df/abstract_building.h"
#include "df/caste_raw.h"
#include "df/creature_interaction_effect_body_transformationst.h"
#include "df/creature_raw.h"
#include "df/entity_position.h"
#include "df/entity_position_assignment.h"
#include "df/histfig_entity_link.h"
#include "df/histfig_entity_link_former_occupationst.h"
#include "df/histfig_entity_link_former_positionst.h"
#include "df/histfig_entity_link_former_squadst.h"
#include "df/histfig_entity_link_occupationst.h"
#include "df/histfig_entity_link_position_claimst.h"
#include "df/histfig_entity_link_positionst.h"
#include "df/histfig_entity_link_squadst.h"
#include "df/histfig_hf_link.h"
#include "df/histfig_site_link.h"
#include "df/historical_entity.h"
#include "df/historical_figure.h"
#include "df/historical_figure_info.h"
#include "df/interaction_effect_add_syndromest.h"
#include "df/interaction_effect_animatest.h"
#include "df/occupation.h"
#include "df/squad.h"
#include "df/syndrome.h"
#include "df/world_site.h"

bool WebLegends::render_figure(std::ostream & s, int32_t id, int32_t page)
{
    CoreSuspender suspend;

    auto hf = df::historical_figure::find(id);
    if (hf == nullptr)
    {
        return false;
    }

    auto race = df::creature_raw::find(hf->race);
    auto caste = (race && hf->caste != -1) ? race->caste.at(hf->caste) : nullptr;

    simple_header(s, hf);

    s << "<p>";
    categorize(s, hf);
    spheres(s, hf);
    born_died(s, hf);
    if (caste)
    {
        s << "<br>" << caste->description;
    }
    if (hf->info && hf->info->curse)
    {
        for (auto it = hf->info->curse->active_effects.begin(); it != hf->info->curse->active_effects.end(); it++)
        {
            std::vector<df::syndrome *> *syndromes = nullptr;
            if (auto syndrome = virtual_cast<df::interaction_effect_add_syndromest>(*it))
            {
                syndromes = &syndrome->syndrome;
            }
            else if (auto zombie = virtual_cast<df::interaction_effect_animatest>(*it))
            {
                syndromes = &zombie->syndrome;
            }
            else
            {
                continue;
            }
            for (auto it2 = syndromes->begin(); it2 != syndromes->end(); it2++)
            {
                for (auto it3 = (*it2)->ce.begin(); it3 != (*it2)->ce.end(); it3++)
                {
                    if (auto transformation = virtual_cast<df::creature_interaction_effect_body_transformationst>(*it3))
                    {
                        auto t_race = df::creature_raw::find(transformation->race);
                        auto t_caste = (t_race && transformation->caste != -1) ? t_race->caste.at(transformation->caste) : nullptr;
                        if (t_caste != nullptr)
                        {
                            s << "<br>" << t_caste->description;
                        }
                    }
                }
            }
        }
    }
    s << "</p>";

    if (!hf->histfig_links.empty())
    {
        s << "<h2 id=\"related-figures\">Related Figures</h2><ul>";
        for (auto it = hf->histfig_links.begin(); it != hf->histfig_links.end(); it++)
        {
            if (auto target = df::historical_figure::find((*it)->target_hf))
            {
                s << "<li>";
                link(s, target);
                BEFORE_SWITCH(link_type, (*it)->getType());
                switch (link_type)
                {
                case histfig_hf_link_type::MOTHER:
                    s << ", mother";
                    BREAK(link_type);
                case histfig_hf_link_type::FATHER:
                    s << ", father";
                    BREAK(link_type);
                case histfig_hf_link_type::SPOUSE:
                    if (target->sex == 0)
                    {
                        s << ", wife";
                    }
                    else if (target->sex == 1)
                    {
                        s << ", husband";
                    }
                    else
                    {
                        s << ", spouse";
                    }
                    BREAK(link_type);
                case histfig_hf_link_type::CHILD:
                    if (target->sex == 0)
                    {
                        s << ", daughter";
                    }
                    else if (target->sex == 1)
                    {
                        s << ", son";
                    }
                    else
                    {
                        s << ", child";
                    }
                    BREAK(link_type);
                case histfig_hf_link_type::LOVER:
                    s << ", lover";
                    BREAK(link_type);
                case histfig_hf_link_type::DEITY:
                    s << ", object of";
                    if ((*it)->link_strength < 10)
                    {
                        s << " dubious";
                    }
                    else if ((*it)->link_strength < 25)
                    {
                        s << " casual";
                    }
                    else if ((*it)->link_strength < 75)
                    {
                        // statistically average worship, I guess?
                    }
                    else if ((*it)->link_strength < 90)
                    {
                        s << " faithful";
                    }
                    else
                    {
                        s << " ardent";
                    }
                    s << " worship";
                    BREAK(link_type);
                case histfig_hf_link_type::COMPANION:
                    s << ", companion";
                    BREAK(link_type);
                case histfig_hf_link_type::PRISONER:
                    s << ", prisoner";
                    BREAK(link_type);
                case histfig_hf_link_type::IMPRISONER:
                    s << ", imprisoner";
                    BREAK(link_type);
                case histfig_hf_link_type::MASTER:
                    s << ", master";
                    BREAK(link_type);
                case histfig_hf_link_type::APPRENTICE:
                    s << ", apprentice";
                    BREAK(link_type);
                case histfig_hf_link_type::FORMER_MASTER:
                    s << ", former master";
                    BREAK(link_type);
                case histfig_hf_link_type::FORMER_APPRENTICE:
                    s << ", former apprentice";
                    BREAK(link_type);
                case histfig_hf_link_type::PET_OWNER:
                    s << ", owner";
                    BREAK(link_type);
                }
                AFTER_SWITCH(link_type, stl_sprintf("fig-%d target=fig-%d", id, (*it)->target_hf));
                born_died(s, target);
                s << "</li>";
            }
        }
        s << "</ul>";
    }
    if (!hf->entity_links.empty())
    {
        s << "<h2 id=\"related-entities\">Related Entities</h2><ul>";
        for (auto it = hf->entity_links.begin(); it != hf->entity_links.end(); it++)
        {
            if (auto ent = df::historical_entity::find((*it)->entity_id))
            {
                s << "<li>";
                link(s, ent);
                BEFORE_SWITCH(link_type, (*it)->getType());
                switch (link_type)
                {
                case histfig_entity_link_type::MEMBER:
                    s << ", member";
                    BREAK(link_type);
                case histfig_entity_link_type::FORMER_MEMBER:
                    s << ", former member";
                    BREAK(link_type);
                case histfig_entity_link_type::MERCENARY:
                    s << ", mercenary";
                    BREAK(link_type);
                case histfig_entity_link_type::FORMER_MERCENARY:
                    s << ", former mercenary";
                    BREAK(link_type);
                case histfig_entity_link_type::SLAVE:
                    s << ", slave";
                    BREAK(link_type);
                case histfig_entity_link_type::FORMER_SLAVE:
                    s << ", former slave";
                    BREAK(link_type);
                case histfig_entity_link_type::PRISONER:
                    s << ", prisoner";
                    BREAK(link_type);
                case histfig_entity_link_type::FORMER_PRISONER:
                    s << ", former prisoner";
                    BREAK(link_type);
                case histfig_entity_link_type::ENEMY:
                    s << ", enemy";
                    BREAK(link_type);
                case histfig_entity_link_type::CRIMINAL:
                    s << ", criminal";
                    BREAK(link_type);
                case histfig_entity_link_type::POSITION:
                {
                    auto l = virtual_cast<df::histfig_entity_link_positionst>(*it);
                    auto assignment = binsearch_in_vector(ent->positions.assignments, l->assignment_id);
                    auto position = binsearch_in_vector(ent->positions.own, assignment->position_id);
                    s << ", ";
                    if (hf->sex == 0 && !position->name_female[0].empty())
                    {
                        s << position->name_female[0];
                    }
                    else if (hf->sex == 1 && !position->name_male[0].empty())
                    {
                        s << position->name_male[0];
                    }
                    else
                    {
                        s << position->name[0];
                    }
                    if (l->start_year > 0)
                    {
                        s << " (since " << l->start_year << ")";
                    }
                    BREAK(link_type);
                }
                case histfig_entity_link_type::FORMER_POSITION:
                {
                    auto l = virtual_cast<df::histfig_entity_link_former_positionst>(*it);
                    auto assignment = binsearch_in_vector(ent->positions.assignments, l->assignment_id);
                    auto position = binsearch_in_vector(ent->positions.own, assignment->position_id);
                    s << ", ";
                    if (hf->sex == 0 && !position->name_female[0].empty())
                    {
                        s << position->name_female[0];
                    }
                    else if (hf->sex == 1 && !position->name_male[0].empty())
                    {
                        s << position->name_male[0];
                    }
                    else
                    {
                        s << position->name[0];
                    }
                    if (l->start_year > 0)
                    {
                        s << " (" << l->start_year << "-" << l->end_year << ")";
                    }
                    else
                    {
                        s << " (until " << l->end_year << ")";
                    }
                    BREAK(link_type);
                }
                case histfig_entity_link_type::POSITION_CLAIM:
                {
                    auto l = virtual_cast<df::histfig_entity_link_position_claimst>(*it);
                    auto assignment = binsearch_in_vector(ent->positions.assignments, l->assignment_id);
                    auto position = binsearch_in_vector(ent->positions.own, assignment->position_id);
                    s << ", ";
                    if (hf->sex == 0 && !position->name_female[0].empty())
                    {
                        s << position->name_female[0];
                    }
                    else if (hf->sex == 1 && !position->name_male[0].empty())
                    {
                        s << position->name_male[0];
                    }
                    else
                    {
                        s << position->name[0];
                    }
                    if (l->start_year > 0)
                    {
                        s << " (since " << l->start_year << ")";
                    }
                    BREAK(link_type);
                }
                case histfig_entity_link_type::SQUAD:
                {
                    auto l = virtual_cast<df::histfig_entity_link_squadst>(*it);
                    auto squad = df::squad::find(l->squad_id);
                    s << ", member of ";
                    if (squad->alias.empty())
                    {
                        name_translated(s, squad->name);
                    }
                    else
                    {
                        s << squad->alias;
                    }
                    if (l->start_year > 0)
                    {
                        s << " (since " << l->start_year << ")";
                    }
                    BREAK(link_type);
                }
                case histfig_entity_link_type::FORMER_SQUAD:
                {
                    auto l = virtual_cast<df::histfig_entity_link_former_squadst>(*it);
                    auto squad = df::squad::find(l->squad_id);
                    s << ", former member of ";
                    if (squad->alias.empty())
                    {
                        name_translated(s, squad->name);
                    }
                    else
                    {
                        s << squad->alias;
                    }
                    if (l->start_year > 0)
                    {
                        s << " (" << l->start_year << "-" << l->end_year << ")";
                    }
                    else
                    {
                        s << " (until " << l->end_year << ")";
                    }
                    BREAK(link_type);
                }
                case histfig_entity_link_type::OCCUPATION:
                {
                    auto l = virtual_cast<df::histfig_entity_link_occupationst>(*it);
                    auto occupation = df::occupation::find(l->occupation_id);
                    auto site = df::world_site::find(occupation->site_id);
                    auto location = binsearch_in_vector(site->buildings, occupation->location_id);
                    s << ", ";
                    BEFORE_SWITCH(occ, occupation->type);
                    switch (occ)
                    {
                    case occupation_type::TAVERN_KEEPER:
                        s << "tavern keeper";
                        BREAK(occ);
                    case occupation_type::PERFORMER:
                        s << "performer";
                        BREAK(occ);
                    case occupation_type::SCHOLAR:
                        s << "scholar";
                        BREAK(occ);
                    case occupation_type::MERCENARY:
                        s << "mercenary";
                        BREAK(occ);
                    case occupation_type::MONSTER_SLAYER:
                        s << "monster slayer";
                        BREAK(occ);
                    case occupation_type::SCRIBE:
                        s << "scribe";
                        BREAK(occ);
                    }
                    AFTER_SWITCH(occ, stl_sprintf("fig-%d (current) occ=%d loc=site-%d/bld-%d", id, l->occupation_id, occupation->site_id, occupation->location_id));
                    s << " at ";
                    link(s, location);
                    s << " (" << l->start_year << "-)";
                    BREAK(link_type);
                }
                case histfig_entity_link_type::FORMER_OCCUPATION:
                {
                    auto l = virtual_cast<df::histfig_entity_link_former_occupationst>(*it);
                    auto occupation = df::occupation::find(l->occupation_id);
                    auto site = df::world_site::find(occupation->site_id);
                    auto location = binsearch_in_vector(site->buildings, occupation->location_id);
                    s << ", former ";
                    BEFORE_SWITCH(occ, occupation->type);
                    switch (occ)
                    {
                    case occupation_type::TAVERN_KEEPER:
                        s << "tavern keeper";
                        BREAK(occ);
                    case occupation_type::PERFORMER:
                        s << "performer";
                        BREAK(occ);
                    case occupation_type::SCHOLAR:
                        s << "scholar";
                        BREAK(occ);
                    case occupation_type::MERCENARY:
                        s << "mercenary";
                        BREAK(occ);
                    case occupation_type::MONSTER_SLAYER:
                        s << "monster slayer";
                        BREAK(occ);
                    case occupation_type::SCRIBE:
                        s << "scribe";
                        BREAK(occ);
                    }
                    AFTER_SWITCH(occ, stl_sprintf("fig-%d (former) occ=%d loc=site-%d/bld-%d", id, l->occupation_id, occupation->site_id, occupation->location_id));
                    s << " at ";
                    link(s, location);
                    s << " (" << l->start_year << "-" << l->end_year << ")";
                    BREAK(link_type);
                }
                }
                AFTER_SWITCH(link_type, stl_sprintf("fig-%d target=ent-%d", id, (*it)->entity_id));
                s << "</li>";
            }
        }
        s << "</ul>";
    }
    if (!hf->site_links.empty())
    {
        s << "<h2 id=\"related-sites\">Related Sites</h2><ul>";
        for (auto it = hf->site_links.begin(); it != hf->site_links.end(); it++)
        {
            auto site = df::world_site::find((*it)->site);
            s << "<li>";
            BEFORE_SWITCH(link_type, (*it)->getType());
            switch (link_type)
            {
            case histfig_site_link_type::OCCUPATION:
                // TODO: int32_t sub_id;
                link(s, site);
                s << ", occupation";
                BREAK(link_type);
            case histfig_site_link_type::SEAT_OF_POWER:
                link(s, site);
                s << ", seat of power";
                BREAK(link_type);
            case histfig_site_link_type::HANGOUT:
                link(s, site);
                s << ", hangout";
                BREAK(link_type);
            case histfig_site_link_type::HOME_SITE_ABSTRACT_BUILDING:
                if (auto structure = binsearch_in_vector(site->buildings, (*it)->sub_id))
                {
                    link(s, structure);
                    s << " in ";
                }
                link(s, site);
                s << ", home";
                BREAK(link_type);
            case histfig_site_link_type::HOME_SITE_REALIZATION_BUILDING:
                // TODO: int32_t sub_id;
                link(s, site);
                s << ", home";
                BREAK(link_type);
            case histfig_site_link_type::LAIR:
                link(s, site);
                s << ", lair";
                BREAK(link_type);
            case histfig_site_link_type::HOME_SITE_REALIZATION_SUL:
                link(s, site);
                s << ", home"; // TODO: roost?
                BREAK(link_type);
            case histfig_site_link_type::HOME_SITE_SAVED_CIVZONE:
                link(s, site);
                s << ", home";
                BREAK(link_type);
            }
            AFTER_SWITCH(link_type, stl_sprintf("fig-%d target=site-%d", id, (*it)->site));
            s << "</li>";
        }
        s << "</ul>";
    }
    int32_t last_page;
    if (!history(s, hf, page, last_page))
    {
        return false;
    }
    pagination(s, stl_sprintf("fig-%d", id), "", "?page=", page, last_page);
    s << "</body></html>";
    return true;
}
