#include "weblegends.h"
#include "helpers.h"

#include "modules/Items.h"
#include "modules/Materials.h"

#include "df/artifact_record.h"
#include "df/creature_raw.h"
#include "df/general_ref_historical_eventst.h"
#include "df/general_ref_knowledge_scholar_flagst.h"
#include "df/general_ref_sitest.h"
#include "df/historical_figure.h"
#include "df/history_event.h"
#include "df/item_constructed.h"
#include "df/item_slabst.h"
#include "df/itemimprovement.h"
#include "df/itemimprovement_itemspecificst.h"
#include "df/itemimprovement_pagesst.h"
#include "df/itemimprovement_writingst.h"
#include "df/knowledge_scholar_flags_0.h"
#include "df/knowledge_scholar_flags_1.h"
#include "df/knowledge_scholar_flags_10.h"
#include "df/knowledge_scholar_flags_11.h"
#include "df/knowledge_scholar_flags_12.h"
#include "df/knowledge_scholar_flags_13.h"
#include "df/knowledge_scholar_flags_2.h"
#include "df/knowledge_scholar_flags_3.h"
#include "df/knowledge_scholar_flags_4.h"
#include "df/knowledge_scholar_flags_5.h"
#include "df/knowledge_scholar_flags_6.h"
#include "df/knowledge_scholar_flags_7.h"
#include "df/knowledge_scholar_flags_8.h"
#include "df/knowledge_scholar_flags_9.h"
#include "df/world_site.h"
#include "df/written_content.h"

static void written_content(std::ostream & s, const event_context & context, df::written_content *content)
{
    if (content->page_start == -1)
    {
        s << " The written contents are a ";
    }
    else
    {
        s << " Pages " << content->page_start << " to " << content->page_end << " are a ";
    }
    typedef std::pair<df::written_content_style, int32_t> style_t;
    std::vector<style_t> styles;
    styles.resize(content->styles.size());
    for (auto style = styles.begin(); style != styles.end(); style++)
    {
        style->first = content->styles.at(style - styles.begin());
        style->second = content->style_strength.at(style - styles.begin());
    }
    list<style_t>(s, styles, [](std::ostream & out, style_t style)
            {
                if (style.second > 1)
                {
                    out << "somewhat ";
                }
                else if (style.second < 1)
                {
                    out << "very ";
                }
                switch (style.first)
                {
                    case written_content_style::Meandering:
                        out << "meandering";
                        break;
                    case written_content_style::Cheerful:
                        out << "cheerful";
                        break;
                    case written_content_style::Depressing:
                        out << "depressing";
                        break;
                    case written_content_style::Rigid:
                        out << "rigid";
                        break;
                    case written_content_style::Serious:
                        out << "serious";
                        break;
                    case written_content_style::Disjointed:
                        out << "disjointed";
                        break;
                    case written_content_style::Ornate:
                        out << "ornate";
                        break;
                    case written_content_style::Forceful:
                        out << "forceful";
                        break;
                    case written_content_style::Humorous:
                        out << "humorous";
                        break;
                    case written_content_style::Immature:
                        out << "immature";
                        break;
                    case written_content_style::SelfIndulgent:
                        out << "self-indulgent";
                        break;
                    case written_content_style::Touching:
                        out << "touching";
                        break;
                    case written_content_style::Compassionate:
                        out << "compassionate";
                        break;
                    case written_content_style::Vicious:
                        out << "vicious";
                        break;
                    case written_content_style::Concise:
                        out << "concise";
                        break;
                    case written_content_style::Scornful:
                        out << "scornful";
                        break;
                    case written_content_style::Witty:
                        out << "witty";
                        break;
                    case written_content_style::Ranting:
                        out << "ranting";
                        break;
                }
            });
    if (!styles.empty())
    {
        s << " ";
    }
    switch (content->type)
    {
        case written_content_type::Manual:
            s << "manual";
            break;
        case written_content_type::Guide:
            s << "guide";
            break;
        case written_content_type::Chronicle:
            s << "chronicle";
            break;
        case written_content_type::ShortStory:
            s << "short story";
            break;
        case written_content_type::Novel:
            s << "novel";
            break;
        case written_content_type::Biography:
            s << "biography";
            break;
        case written_content_type::Autobiography:
            s << "autobiography";
            break;
        case written_content_type::Poem:
            s << "poem";
            break;
        case written_content_type::Play:
            s << "play";
            break;
        case written_content_type::Letter:
            s << "letter";
            break;
        case written_content_type::Essay:
            s << "essay";
            break;
        case written_content_type::Dialog:
            s << "dialog";
            break;
        case written_content_type::MusicalComposition:
            s << "musical composition";
            break;
        case written_content_type::Choreography:
            s << "choreography";
            break;
    }
    if (auto author = df::historical_figure::find(content->author))
    {
        s << " by ";
        link(s, author);
    }
    s << " titled " << content->title << ".";

    for (auto ref = content->refs.begin(); ref != content->refs.end(); ref++)
    {
        switch ((*ref)->getType())
        {
            case general_ref_type::HISTORICAL_EVENT:
                if (auto r = virtual_cast<df::general_ref_historical_eventst>(*ref))
                {
                    if (auto e = df::history_event::find(r->event_id))
                    {
                        int32_t last_year = 0;
                        int32_t last_seconds = -1;
                        s << " ";
                        event(s, context, e, last_year, last_seconds);
                    }
                }
                break;
            case general_ref_type::SITE:
                if (auto r = virtual_cast<df::general_ref_sitest>(*ref))
                {
                    if (auto site = df::world_site::find(r->site_id))
                    {
                        s << " The writing concerns the";
                        categorize(s, site);
                        s << " ";
                        event_link(s, context, site);
                        s << ".";
                    }
                }
                break;
            case general_ref_type::KNOWLEDGE_SCHOLAR_FLAG:
                if (auto r = virtual_cast<df::general_ref_knowledge_scholar_flagst>(*ref))
                {
                    s << " The writing concerns ";
                    switch (r->knowledge_category)
                    {
                        case 0:
                            {
                                df::knowledge_scholar_flags_0 know(r->knowledge_flags);
                                if (know.bits.philosophy_logic_formal_reasoning)
                                {
                                    s << "philosophy_logic_formal_reasoning";
                                }
                                else if (know.bits.philosophy_logic_deductive_reasoning)
                                {
                                    s << "philosophy_logic_deductive_reasoning";
                                }
                                else if (know.bits.philosophy_logic_syllogistic_logic)
                                {
                                    s << "philosophy_logic_syllogistic_logic";
                                }
                                else if (know.bits.philosophy_logic_hypothetical_syllogisms)
                                {
                                    s << "philosophy_logic_hypothetical_syllogisms";
                                }
                                else if (know.bits.philosophy_logic_propositional_logic)
                                {
                                    s << "philosophy_logic_propositional_logic";
                                }
                                else if (know.bits.philosophy_logic_dialectic_reasoning)
                                {
                                    s << "philosophy_logic_dialectic_reasoning";
                                }
                                else if (know.bits.philosophy_logic_analogical_inference)
                                {
                                    s << "philosophy_logic_analogical_inference";
                                }
                                else if (know.bits.philosophy_ethics_applied_medical)
                                {
                                    s << "philosophy_ethics_applied_medical";
                                }
                                else if (know.bits.philosophy_ethics_individual_value)
                                {
                                    s << "philosophy_ethics_individual_value";
                                }
                                else if (know.bits.philosophy_ethics_state_consequentialism)
                                {
                                    s << "philosophy_ethics_state_consequentialism";
                                }
                                else if (know.bits.philosophy_epistemology_truth)
                                {
                                    s << "philosophy_epistemology_truth";
                                }
                                else if (know.bits.philosophy_epistemology_perception)
                                {
                                    s << "philosophy_epistemology_perception";
                                }
                                else if (know.bits.philosophy_epistemology_justification)
                                {
                                    s << "philosophy_epistemology_justification";
                                }
                                else if (know.bits.philosophy_epistemology_belief)
                                {
                                    s << "philosophy_epistemology_belief";
                                }
                                else if (know.bits.philosophy_metaphysics_existence)
                                {
                                    s << "philosophy_metaphysics_existence";
                                }
                                else if (know.bits.philosophy_metaphysics_time)
                                {
                                    s << "philosophy_metaphysics_time";
                                }
                                else if (know.bits.philosophy_metaphysics_mind_body)
                                {
                                    s << "philosophy_metaphysics_mind_body";
                                }
                                else if (know.bits.philosophy_metaphysics_objects_and_properties)
                                {
                                    s << "philosophy_metaphysics_objects_and_properties";
                                }
                                else if (know.bits.philosophy_metaphysics_wholes_and_parts)
                                {
                                    s << "philosophy_metaphysics_wholes_and_parts";
                                }
                                else if (know.bits.philosophy_metaphysics_events)
                                {
                                    s << "philosophy_metaphysics_events";
                                }
                                else if (know.bits.philosophy_metaphysics_processes)
                                {
                                    s << "philosophy_metaphysics_processes";
                                }
                                else if (know.bits.philosophy_metaphysics_causation)
                                {
                                    s << "philosophy_metaphysics_causation";
                                }
                                else if (know.bits.philosophy_ethics_applied_military)
                                {
                                    s << "philosophy_ethics_applied_military";
                                }
                                else if (know.bits.philosophy_ethics_applied_interpersonal_conduct)
                                {
                                    s << "philosophy_ethics_applied_interpersonal_conduct";
                                }
                                else if (know.bits.philosophy_specialized_law)
                                {
                                    s << "philosophy_specialized_law";
                                }
                                else if (know.bits.philosophy_specialized_education)
                                {
                                    s << "philosophy_specialized_education";
                                }
                                else if (know.bits.philosophy_specialized_language_grammar)
                                {
                                    s << "philosophy_specialized_language_grammar";
                                }
                                else if (know.bits.philosophy_specialized_language_etymology)
                                {
                                    s << "philosophy_specialized_language_etymology";
                                }
                                else if (know.bits.philosophy_specialized_politics_diplomacy)
                                {
                                    s << "philosophy_specialized_politics_diplomacy";
                                }
                                else if (know.bits.philosophy_specialized_politics_government_forms)
                                {
                                    s << "philosophy_specialized_politics_government_forms";
                                }
                                else if (know.bits.philosophy_specialized_politics_economic_policy)
                                {
                                    s << "philosophy_specialized_politics_economic_policy";
                                }
                                else if (know.bits.philosophy_specialized_politics_social_welfare)
                                {
                                    s << "philosophy_specialized_politics_social_welfare";
                                }
                            }
                            break;
                        case 1:
                            {
                                df::knowledge_scholar_flags_1 know(r->knowledge_flags);
                                if (know.bits.philosophy_logic_inductive_reasoning)
                                {
                                    s << "philosophy_logic_inductive_reasoning";
                                }
                                else if (know.bits.philosophy_logic_direct_inference)
                                {
                                    s << "philosophy_logic_direct_inference";
                                }
                                else if (know.bits.philosophy_aesthetics_nature_of_beauty)
                                {
                                    s << "philosophy_aesthetics_nature_of_beauty";
                                }
                                else if (know.bits.philosophy_aesthetics_value_of_art)
                                {
                                    s << "philosophy_aesthetics_value_of_art";
                                }
                                else if (know.bits.philosophy_specialized_language_dictionary)
                                {
                                    s << "philosophy_specialized_language_dictionary";
                                }
                            }
                            break;
                        case 2:
                            {
                                df::knowledge_scholar_flags_2 know(r->knowledge_flags);
                                if (know.bits.mathematics_method_proof_by_contradiction)
                                {
                                    s << "mathematics_method_proof_by_contradiction";
                                }
                                else if (know.bits.mathematics_notation_zero)
                                {
                                    s << "mathematics_notation_zero";
                                }
                                else if (know.bits.mathematics_notation_negative_numbers)
                                {
                                    s << "mathematics_notation_negative_numbers";
                                }
                                else if (know.bits.mathematics_notation_large_numbers)
                                {
                                    s << "mathematics_notation_large_numbers";
                                }
                                else if (know.bits.mathematics_notation_positional)
                                {
                                    s << "mathematics_notation_positional";
                                }
                                else if (know.bits.mathematics_geometry_basic_objects)
                                {
                                    s << "mathematics_geometry_basic_objects";
                                }
                                else if (know.bits.mathematics_method_exhaustion)
                                {
                                    s << "mathematics_method_exhaustion";
                                }
                                else if (know.bits.mathematics_geometry_similar_and_congruent_triangles)
                                {
                                    s << "mathematics_geometry_similar_and_congruent_triangles";
                                }
                                else if (know.bits.mathematics_geometry_geometric_mean_theorem)
                                {
                                    s << "mathematics_geometry_geometric_mean_theorem";
                                }
                                else if (know.bits.mathematics_geometry_isosceles_base_angles_equal)
                                {
                                    s << "mathematics_geometry_isosceles_base_angles_equal";
                                }
                                else if (know.bits.mathematics_geometry_inscribed_triangle_on_diameter_is_right)
                                {
                                    s << "mathematics_geometry_inscribed_triangle_on_diameter_is_right";
                                }
                                else if (know.bits.mathematics_geometry_pythagorean_theorem)
                                {
                                    s << "mathematics_geometry_pythagorean_theorem";
                                }
                                else if (know.bits.mathematics_geometry_pythagorean_triples_small)
                                {
                                    s << "mathematics_geometry_pythagorean_triples_small";
                                }
                                else if (know.bits.mathematics_geometry_pythagorean_triples_3_digit)
                                {
                                    s << "mathematics_geometry_pythagorean_triples_3_digit";
                                }
                                else if (know.bits.mathematics_geometry_pythagorean_triples_4_digit)
                                {
                                    s << "mathematics_geometry_pythagorean_triples_4_digit";
                                }
                                else if (know.bits.mathematics_geometry_existence_of_incommensurable_ratios)
                                {
                                    s << "mathematics_geometry_existence_of_incommensurable_ratios";
                                }
                                else if (know.bits.mathematics_method_axiomatic_reasoning)
                                {
                                    s << "mathematics_method_axiomatic_reasoning";
                                }
                                else if (know.bits.mathematics_numbers_unique_prime_factorization)
                                {
                                    s << "mathematics_numbers_unique_prime_factorization";
                                }
                                else if (know.bits.mathematics_numbers_algorithm_for_computing_gcd)
                                {
                                    s << "mathematics_numbers_algorithm_for_computing_gcd";
                                }
                                else if (know.bits.mathematics_geometry_volume_of_pyramid)
                                {
                                    s << "mathematics_geometry_volume_of_pyramid";
                                }
                                else if (know.bits.mathematics_geometry_volume_of_cone)
                                {
                                    s << "mathematics_geometry_volume_of_cone";
                                }
                                else if (know.bits.mathematics_geometry_volume_of_sphere)
                                {
                                    s << "mathematics_geometry_volume_of_sphere";
                                }
                                else if (know.bits.mathematics_geometry_pi_to_4_digits)
                                {
                                    s << "mathematics_geometry_pi_to_4_digits";
                                }
                                else if (know.bits.mathematics_numbers_division_algorithm)
                                {
                                    s << "mathematics_numbers_division_algorithm";
                                }
                                else if (know.bits.mathematics_geometry_table_of_chord_values)
                                {
                                    s << "mathematics_geometry_table_of_chord_values";
                                }
                                else if (know.bits.mathematics_geometry_area_of_triangle_from_side_lengths)
                                {
                                    s << "mathematics_geometry_area_of_triangle_from_side_lengths";
                                }
                                else if (know.bits.mathematics_geometry_area_of_circle)
                                {
                                    s << "mathematics_geometry_area_of_circle";
                                }
                                else if (know.bits.mathematics_geometry_pi_to_6_digits)
                                {
                                    s << "mathematics_geometry_pi_to_6_digits";
                                }
                                else if (know.bits.mathematics_geometry_definitions_and_basic_properties_of_conic_sections)
                                {
                                    s << "mathematics_geometry_definitions_and_basic_properties_of_conic_sections";
                                }
                                else if (know.bits.mathematics_numbers_chinese_remainder_algorithm)
                                {
                                    s << "mathematics_numbers_chinese_remainder_algorithm";
                                }
                                else if (know.bits.mathematics_geometry_area_enclosed_by_line_and_parabola)
                                {
                                    s << "mathematics_geometry_area_enclosed_by_line_and_parabola";
                                }
                                else if (know.bits.mathematics_numbers_sieve_algorithm_for_primes)
                                {
                                    s << "mathematics_numbers_sieve_algorithm_for_primes";
                                }
                            }
                            break;
                        case 3:
                            {
                                df::knowledge_scholar_flags_3 know(r->knowledge_flags);
                                if (know.bits.mathematics_numbers_root_2_to_5_digits)
                                {
                                    s << "mathematics_numbers_root_2_to_5_digits";
                                }
                                else if (know.bits.mathematics_numbers_infinite_primes)
                                {
                                    s << "mathematics_numbers_infinite_primes";
                                }
                                else if (know.bits.mathematics_numbers_root_2_irrational)
                                {
                                    s << "mathematics_numbers_root_2_irrational";
                                }
                                else if (know.bits.mathematics_geometry_surface_area_of_sphere)
                                {
                                    s << "mathematics_geometry_surface_area_of_sphere";
                                }
                                else if (know.bits.mathematics_algebra_finite_summation_formulas)
                                {
                                    s << "mathematics_algebra_finite_summation_formulas";
                                }
                                else if (know.bits.mathematics_algebra_solving_linear_systems)
                                {
                                    s << "mathematics_algebra_solving_linear_systems";
                                }
                                else if (know.bits.mathematics_algebra_balancing_and_completion)
                                {
                                    s << "mathematics_algebra_balancing_and_completion";
                                }
                                else if (know.bits.mathematics_algebra_quadratic_by_completing_square)
                                {
                                    s << "mathematics_algebra_quadratic_by_completing_square";
                                }
                                else if (know.bits.mathematics_algebra_quadratic_formula)
                                {
                                    s << "mathematics_algebra_quadratic_formula";
                                }
                                else if (know.bits.mathematics_notation_syncopated_algebra)
                                {
                                    s << "mathematics_notation_syncopated_algebra";
                                }
                                else if (know.bits.mathematics_geometry_law_of_sines)
                                {
                                    s << "mathematics_geometry_law_of_sines";
                                }
                                else if (know.bits.mathematics_geometry_angle_sum_difference_trig_identities)
                                {
                                    s << "mathematics_geometry_angle_sum_difference_trig_identities";
                                }
                                else if (know.bits.mathematics_algebra_pascals_triangle)
                                {
                                    s << "mathematics_algebra_pascals_triangle";
                                }
                                else if (know.bits.mathematics_algebra_solving_higher_order_polynomials)
                                {
                                    s << "mathematics_algebra_solving_higher_order_polynomials";
                                }
                                else if (know.bits.mathematics_notation_early_symbols_for_operations)
                                {
                                    s << "mathematics_notation_early_symbols_for_operations";
                                }
                                else if (know.bits.mathematics_algebra_divergence_of_harmonic_series)
                                {
                                    s << "mathematics_algebra_divergence_of_harmonic_series";
                                }
                                else if (know.bits.mathematics_geometry_properties_of_chords)
                                {
                                    s << "mathematics_geometry_properties_of_chords";
                                }
                            }
                            break;
                        case 4:
                            {
                                df::knowledge_scholar_flags_4 know(r->knowledge_flags);
                                if (know.bits.history_sourcing_basic_reliability)
                                {
                                    s << "history_sourcing_basic_reliability";
                                }
                                else if (know.bits.history_sourcing_role_of_systemic_bias)
                                {
                                    s << "history_sourcing_role_of_systemic_bias";
                                }
                                else if (know.bits.history_sourcing_role_of_state_bias_and_propaganda)
                                {
                                    s << "history_sourcing_role_of_state_bias_and_propaganda";
                                }
                                else if (know.bits.history_sourcing_personal_interviews)
                                {
                                    s << "history_sourcing_personal_interviews";
                                }
                                else if (know.bits.history_theory_historical_causation)
                                {
                                    s << "history_theory_historical_causation";
                                }
                                else if (know.bits.history_theory_historical_cycles)
                                {
                                    s << "history_theory_historical_cycles";
                                }
                                else if (know.bits.history_theory_social_cohesion)
                                {
                                    s << "history_theory_social_cohesion";
                                }
                                else if (know.bits.history_theory_social_conflict)
                                {
                                    s << "history_theory_social_conflict";
                                }
                                else if (know.bits.history_form_biography)
                                {
                                    s << "history_form_biography";
                                }
                                else if (know.bits.history_form_comparative_biography)
                                {
                                    s << "history_form_comparative_biography";
                                }
                                else if (know.bits.history_form_biographical_dictionaries)
                                {
                                    s << "history_form_biographical_dictionaries";
                                }
                                else if (know.bits.history_form_autobiographical_adventure)
                                {
                                    s << "history_form_autobiographical_adventure";
                                }
                                else if (know.bits.history_form_genealogy)
                                {
                                    s << "history_form_genealogy";
                                }
                                else if (know.bits.history_form_encyclopedia)
                                {
                                    s << "history_form_encyclopedia";
                                }
                                else if (know.bits.history_form_cultural_history)
                                {
                                    s << "history_form_cultural_history";
                                }
                                else if (know.bits.history_form_cultural_comparison)
                                {
                                    s << "history_form_cultural_comparison";
                                }
                                else if (know.bits.history_sourcing_role_of_cultural_differences)
                                {
                                    s << "history_sourcing_role_of_cultural_differences";
                                }
                                else if (know.bits.history_form_alternate_history)
                                {
                                    s << "history_form_alternate_history";
                                }
                                else if (know.bits.history_sourcing_basic_archaeology)
                                {
                                    s << "history_sourcing_basic_archaeology";
                                }
                                else if (know.bits.history_form_treatise_on_tech_evolution)
                                {
                                    s << "history_form_treatise_on_tech_evolution";
                                }
                            }
                            break;
                        case 5:
                            {
                                df::knowledge_scholar_flags_5 know(r->knowledge_flags);
                                if (know.bits.astronomy_phases_of_the_moon)
                                {
                                    s << "astronomy_phases_of_the_moon";
                                }
                                else if (know.bits.astronomy_summer_winter_moon)
                                {
                                    s << "astronomy_summer_winter_moon";
                                }
                                else if (know.bits.astronomy_path_of_the_moon)
                                {
                                    s << "astronomy_path_of_the_moon";
                                }
                                else if (know.bits.astronomy_tides_and_the_moon)
                                {
                                    s << "astronomy_tides_and_the_moon";
                                }
                                else if (know.bits.astronomy_height_of_tides_vs_moon_and_sun)
                                {
                                    s << "astronomy_height_of_tides_vs_moon_and_sun";
                                }
                                else if (know.bits.astronomy_summer_winter_sun)
                                {
                                    s << "astronomy_summer_winter_sun";
                                }
                                else if (know.bits.astronomy_relationship_between_lunar_solar_year)
                                {
                                    s << "astronomy_relationship_between_lunar_solar_year";
                                }
                                else if (know.bits.astronomy_daylight_variation_with_solar_year)
                                {
                                    s << "astronomy_daylight_variation_with_solar_year";
                                }
                                else if (know.bits.astronomy_geocentric_model)
                                {
                                    s << "astronomy_geocentric_model";
                                }
                                else if (know.bits.astronomy_heliocentric_model)
                                {
                                    s << "astronomy_heliocentric_model";
                                }
                                else if (know.bits.astronomy_dates_of_lunar_and_solar_eclipses)
                                {
                                    s << "astronomy_dates_of_lunar_and_solar_eclipses";
                                }
                                else if (know.bits.astronomy_star_charts)
                                {
                                    s << "astronomy_star_charts";
                                }
                                else if (know.bits.astronomy_star_catalogues_100)
                                {
                                    s << "astronomy_star_catalogues_100";
                                }
                                else if (know.bits.astronomy_star_catalogues_1000)
                                {
                                    s << "astronomy_star_catalogues_1000";
                                }
                                else if (know.bits.astronomy_star_color_classification)
                                {
                                    s << "astronomy_star_color_classification";
                                }
                                else if (know.bits.astronomy_star_magnitude_classification)
                                {
                                    s << "astronomy_star_magnitude_classification";
                                }
                                else if (know.bits.astronomy_shape_of_the_world)
                                {
                                    s << "astronomy_shape_of_the_world";
                                }
                                else if (know.bits.astronomy_precession_of_equinoxes)
                                {
                                    s << "astronomy_precession_of_equinoxes";
                                }
                                else if (know.bits.astronomy_method_empirical_observation)
                                {
                                    s << "astronomy_method_empirical_observation";
                                }
                                else if (know.bits.astronomy_method_path_models)
                                {
                                    s << "astronomy_method_path_models";
                                }
                            }
                            break;
                        case 6:
                            {
                                df::knowledge_scholar_flags_6 know(r->knowledge_flags);
                                if (know.bits.naturalist_method_dissection)
                                {
                                    s << "naturalist_method_dissection";
                                }
                                else if (know.bits.naturalist_observation_anatomy)
                                {
                                    s << "naturalist_observation_anatomy";
                                }
                                else if (know.bits.naturalist_theory_comparative_anatomy)
                                {
                                    s << "naturalist_theory_comparative_anatomy";
                                }
                                else if (know.bits.naturalist_theory_classification_by_physical_features)
                                {
                                    s << "naturalist_theory_classification_by_physical_features";
                                }
                                else if (know.bits.naturalist_observation_migration_patterns)
                                {
                                    s << "naturalist_observation_migration_patterns";
                                }
                                else if (know.bits.naturalist_observation_reproductive_behavior)
                                {
                                    s << "naturalist_observation_reproductive_behavior";
                                }
                                else if (know.bits.naturalist_observation_foraging_behavior_and_diet)
                                {
                                    s << "naturalist_observation_foraging_behavior_and_diet";
                                }
                                else if (know.bits.naturalist_theory_food_chain)
                                {
                                    s << "naturalist_theory_food_chain";
                                }
                                else if (know.bits.naturalist_observation_social_behavior)
                                {
                                    s << "naturalist_observation_social_behavior";
                                }
                                else if (know.bits.naturalist_observation_diseases)
                                {
                                    s << "naturalist_observation_diseases";
                                }
                                else if (know.bits.naturalist_theory_climactic_adaptation)
                                {
                                    s << "naturalist_theory_climactic_adaptation";
                                }
                                else if (know.bits.naturalist_observation_embriological_development)
                                {
                                    s << "naturalist_observation_embriological_development";
                                }
                                else if (know.bits.naturalist_theory_struggle_for_existence)
                                {
                                    s << "naturalist_theory_struggle_for_existence";
                                }
                            }
                            break;
                        case 7:
                            {
                                df::knowledge_scholar_flags_7 know(r->knowledge_flags);
                                if (know.bits.chemistry_classification_combustibles)
                                {
                                    s << "chemistry_classification_combustibles";
                                }
                                else if (know.bits.chemistry_classification_ores)
                                {
                                    s << "chemistry_classification_ores";
                                }
                                else if (know.bits.chemistry_metallurgy_alloys)
                                {
                                    s << "chemistry_metallurgy_alloys";
                                }
                                else if (know.bits.chemistry_classification_scratch_test)
                                {
                                    s << "chemistry_classification_scratch_test";
                                }
                                else if (know.bits.chemistry_classification_elemental_theory)
                                {
                                    s << "chemistry_classification_elemental_theory";
                                }
                                else if (know.bits.chemistry_chemicals_adhesives)
                                {
                                    s << "chemistry_chemicals_adhesives";
                                }
                                else if (know.bits.chemistry_laboratory_blast_furnace)
                                {
                                    s << "chemistry_laboratory_blast_furnace";
                                }
                                else if (know.bits.chemistry_laboratory_alembic)
                                {
                                    s << "chemistry_laboratory_alembic";
                                }
                                else if (know.bits.chemistry_laboratory_theory_of_liquid_liquid_extraction)
                                {
                                    s << "chemistry_laboratory_theory_of_liquid_liquid_extraction";
                                }
                                else if (know.bits.chemistry_laboratory_theory_of_distillation)
                                {
                                    s << "chemistry_laboratory_theory_of_distillation";
                                }
                                else if (know.bits.chemistry_laboratory_theory_of_evaporation)
                                {
                                    s << "chemistry_laboratory_theory_of_evaporation";
                                }
                                else if (know.bits.chemistry_classification_alkali_and_acids)
                                {
                                    s << "chemistry_classification_alkali_and_acids";
                                }
                                else if (know.bits.chemistry_laboratory_systematic_experiments)
                                {
                                    s << "chemistry_laboratory_systematic_experiments";
                                }
                                else if (know.bits.chemistry_laboratory_glass_flask)
                                {
                                    s << "chemistry_laboratory_glass_flask";
                                }
                                else if (know.bits.chemistry_laboratory_glass_beaker)
                                {
                                    s << "chemistry_laboratory_glass_beaker";
                                }
                                else if (know.bits.chemistry_laboratory_glass_vial)
                                {
                                    s << "chemistry_laboratory_glass_vial";
                                }
                                else if (know.bits.chemistry_laboratory_glass_funnel)
                                {
                                    s << "chemistry_laboratory_glass_funnel";
                                }
                                else if (know.bits.chemistry_laboratory_crucible)
                                {
                                    s << "chemistry_laboratory_crucible";
                                }
                                else if (know.bits.chemistry_chemicals_nitric_acid)
                                {
                                    s << "chemistry_chemicals_nitric_acid";
                                }
                                else if (know.bits.chemistry_chemicals_sulfuric_acid)
                                {
                                    s << "chemistry_chemicals_sulfuric_acid";
                                }
                                else if (know.bits.chemistry_chemicals_aqua_regia)
                                {
                                    s << "chemistry_chemicals_aqua_regia";
                                }
                                else if (know.bits.chemistry_laboratory_glass_ampoule)
                                {
                                    s << "chemistry_laboratory_glass_ampoule";
                                }
                                else if (know.bits.chemistry_laboratory_glass_retort)
                                {
                                    s << "chemistry_laboratory_glass_retort";
                                }
                                else if (know.bits.chemistry_laboratory_lab_ovens)
                                {
                                    s << "chemistry_laboratory_lab_ovens";
                                }
                            }
                            break;
                        case 8:
                            {
                                df::knowledge_scholar_flags_8 know(r->knowledge_flags);
                                if (know.bits.geography_surveying_basic)
                                {
                                    s << "geography_surveying_basic";
                                }
                                else if (know.bits.geography_surveying_staff)
                                {
                                    s << "geography_surveying_staff";
                                }
                                else if (know.bits.geography_cartography_basic)
                                {
                                    s << "geography_cartography_basic";
                                }
                                else if (know.bits.geography_surveying_triangulation)
                                {
                                    s << "geography_surveying_triangulation";
                                }
                                else if (know.bits.geography_surveying_cartographical)
                                {
                                    s << "geography_surveying_cartographical";
                                }
                                else if (know.bits.geography_surveying_land)
                                {
                                    s << "geography_surveying_land";
                                }
                                else if (know.bits.geography_surveying_military)
                                {
                                    s << "geography_surveying_military";
                                }
                                else if (know.bits.geography_surveying_engineering)
                                {
                                    s << "geography_surveying_engineering";
                                }
                                else if (know.bits.geography_cartography_geological)
                                {
                                    s << "geography_cartography_geological";
                                }
                                else if (know.bits.geography_cartography_grid_system)
                                {
                                    s << "geography_cartography_grid_system";
                                }
                                else if (know.bits.geography_cartography_distance_scale)
                                {
                                    s << "geography_cartography_distance_scale";
                                }
                                else if (know.bits.geography_cartography_height_measurements)
                                {
                                    s << "geography_cartography_height_measurements";
                                }
                                else if (know.bits.geography_method_economic_data_collection)
                                {
                                    s << "geography_method_economic_data_collection";
                                }
                                else if (know.bits.geography_cartography_economic)
                                {
                                    s << "geography_cartography_economic";
                                }
                                else if (know.bits.geography_form_atlas)
                                {
                                    s << "geography_form_atlas";
                                }
                                else if (know.bits.geography_theory_delta_formation)
                                {
                                    s << "geography_theory_delta_formation";
                                }
                                else if (know.bits.geography_theory_wind_patterns)
                                {
                                    s << "geography_theory_wind_patterns";
                                }
                                else if (know.bits.geography_theory_origin_of_rainfall_from_evap_condense)
                                {
                                    s << "geography_theory_origin_of_rainfall_from_evap_condense";
                                }
                                else if (know.bits.geography_theory_water_cycle)
                                {
                                    s << "geography_theory_water_cycle";
                                }
                                else if (know.bits.geography_theory_latitude_climate_zones)
                                {
                                    s << "geography_theory_latitude_climate_zones";
                                }
                                else if (know.bits.geography_cartography_accurate_maps)
                                {
                                    s << "geography_cartography_accurate_maps";
                                }
                                else if (know.bits.geography_cartography_map_projections)
                                {
                                    s << "geography_cartography_map_projections";
                                }
                            }
                            break;
                        case 9:
                            {
                                df::knowledge_scholar_flags_9 know(r->knowledge_flags);
                                if (know.bits.medicine_theory_disease_and_fouled_water)
                                {
                                    s << "medicine_theory_disease_and_fouled_water";
                                }
                                else if (know.bits.medicine_method_physical_examination)
                                {
                                    s << "medicine_method_physical_examination";
                                }
                                else if (know.bits.medicine_method_autopsy)
                                {
                                    s << "medicine_method_autopsy";
                                }
                                else if (know.bits.medicine_theory_prognosis)
                                {
                                    s << "medicine_theory_prognosis";
                                }
                                else if (know.bits.medicine_tool_herbal_remedies)
                                {
                                    s << "medicine_tool_herbal_remedies";
                                }
                                else if (know.bits.medicine_tool_animal_remedies)
                                {
                                    s << "medicine_tool_animal_remedies";
                                }
                                else if (know.bits.medicine_tool_mineral_remedies)
                                {
                                    s << "medicine_tool_mineral_remedies";
                                }
                                else if (know.bits.medicine_tool_bandages)
                                {
                                    s << "medicine_tool_bandages";
                                }
                                else if (know.bits.medicine_theory_disease_classification)
                                {
                                    s << "medicine_theory_disease_classification";
                                }
                                else if (know.bits.medicine_theory_toxicology)
                                {
                                    s << "medicine_theory_toxicology";
                                }
                                else if (know.bits.medicine_theory_acute_and_chronic_conditions)
                                {
                                    s << "medicine_theory_acute_and_chronic_conditions";
                                }
                                else if (know.bits.medicine_theory_endemic_disease)
                                {
                                    s << "medicine_theory_endemic_disease";
                                }
                                else if (know.bits.medicine_theory_epidemic_disease)
                                {
                                    s << "medicine_theory_epidemic_disease";
                                }
                                else if (know.bits.medicine_theory_exacerbation)
                                {
                                    s << "medicine_theory_exacerbation";
                                }
                                else if (know.bits.medicine_theory_paroxysm)
                                {
                                    s << "medicine_theory_paroxysm";
                                }
                                else if (know.bits.medicine_theory_relapse)
                                {
                                    s << "medicine_theory_relapse";
                                }
                                else if (know.bits.medicine_theory_convalescence)
                                {
                                    s << "medicine_theory_convalescence";
                                }
                                else if (know.bits.medicine_method_treatment_of_traumatic_injuries)
                                {
                                    s << "medicine_method_treatment_of_traumatic_injuries";
                                }
                                else if (know.bits.medicine_method_fracture_treatment)
                                {
                                    s << "medicine_method_fracture_treatment";
                                }
                                else if (know.bits.medicine_theory_fracture_classification)
                                {
                                    s << "medicine_theory_fracture_classification";
                                }
                                else if (know.bits.medicine_tool_traction_bench)
                                {
                                    s << "medicine_tool_traction_bench";
                                }
                                else if (know.bits.medicine_method_fracture_immobilization)
                                {
                                    s << "medicine_method_fracture_immobilization";
                                }
                                else if (know.bits.medicine_tool_orthopedic_cast)
                                {
                                    s << "medicine_tool_orthopedic_cast";
                                }
                                else if (know.bits.medicine_method_surgery_excision)
                                {
                                    s << "medicine_method_surgery_excision";
                                }
                                else if (know.bits.medicine_method_surgery_incision)
                                {
                                    s << "medicine_method_surgery_incision";
                                }
                                else if (know.bits.medicine_method_hernia_surgery)
                                {
                                    s << "medicine_method_hernia_surgery";
                                }
                                else if (know.bits.medicine_method_tracheotomy_surgery)
                                {
                                    s << "medicine_method_tracheotomy_surgery";
                                }
                                else if (know.bits.medicine_method_lithotomy_surgery)
                                {
                                    s << "medicine_method_lithotomy_surgery";
                                }
                                else if (know.bits.medicine_method_surgery_scraping)
                                {
                                    s << "medicine_method_surgery_scraping";
                                }
                                else if (know.bits.medicine_method_surgery_draining)
                                {
                                    s << "medicine_method_surgery_draining";
                                }
                                else if (know.bits.medicine_method_surgery_probing)
                                {
                                    s << "medicine_method_surgery_probing";
                                }
                                else if (know.bits.medicine_method_surgery_suturing)
                                {
                                    s << "medicine_method_surgery_suturing";
                                }
                            }
                            break;
                        case 10:
                            {
                                df::knowledge_scholar_flags_10 know(r->knowledge_flags);
                                if (know.bits.medicine_method_surgery_ligature)
                                {
                                    s << "medicine_method_surgery_ligature";
                                }
                                else if (know.bits.medicine_theory_surgical_models)
                                {
                                    s << "medicine_theory_surgical_models";
                                }
                                else if (know.bits.medicine_tool_mud_bags_as_surgical_models)
                                {
                                    s << "medicine_tool_mud_bags_as_surgical_models";
                                }
                                else if (know.bits.medicine_tool_plants_as_surgical_models)
                                {
                                    s << "medicine_tool_plants_as_surgical_models";
                                }
                                else if (know.bits.medicine_tool_animals_as_surgical_models)
                                {
                                    s << "medicine_tool_animals_as_surgical_models";
                                }
                                else if (know.bits.medicine_theory_specialized_surgical_instruments)
                                {
                                    s << "medicine_theory_specialized_surgical_instruments";
                                }
                                else if (know.bits.medicine_tool_forceps)
                                {
                                    s << "medicine_tool_forceps";
                                }
                                else if (know.bits.medicine_tool_scalpel)
                                {
                                    s << "medicine_tool_scalpel";
                                }
                                else if (know.bits.medicine_tool_surgical_scissors)
                                {
                                    s << "medicine_tool_surgical_scissors";
                                }
                                else if (know.bits.medicine_tool_surgical_needles)
                                {
                                    s << "medicine_tool_surgical_needles";
                                }
                                else if (know.bits.medicine_method_cataract_surgery)
                                {
                                    s << "medicine_method_cataract_surgery";
                                }
                                else if (know.bits.medicine_method_cauterization)
                                {
                                    s << "medicine_method_cauterization";
                                }
                                else if (know.bits.medicine_method_anesthesia)
                                {
                                    s << "medicine_method_anesthesia";
                                }
                                else if (know.bits.medicine_theory_pulmonary_medicine)
                                {
                                    s << "medicine_theory_pulmonary_medicine";
                                }
                                else if (know.bits.medicine_theory_anatomical_studies)
                                {
                                    s << "medicine_theory_anatomical_studies";
                                }
                                else if (know.bits.medicine_theory_classification_of_bodily_fluids)
                                {
                                    s << "medicine_theory_classification_of_bodily_fluids";
                                }
                                else if (know.bits.medicine_theory_eye_anatomy)
                                {
                                    s << "medicine_theory_eye_anatomy";
                                }
                                else if (know.bits.medicine_theory_motor_vs_sensory_nerves)
                                {
                                    s << "medicine_theory_motor_vs_sensory_nerves";
                                }
                                else if (know.bits.medicine_theory_nervous_system_function)
                                {
                                    s << "medicine_theory_nervous_system_function";
                                }
                                else if (know.bits.medicine_theory_reaction_time)
                                {
                                    s << "medicine_theory_reaction_time";
                                }
                                else if (know.bits.medicine_theory_blood_vessels)
                                {
                                    s << "medicine_theory_blood_vessels";
                                }
                                else if (know.bits.medicine_theory_pulmonary_circulation)
                                {
                                    s << "medicine_theory_pulmonary_circulation";
                                }
                                else if (know.bits.medicine_theory_comparative_anatomy)
                                {
                                    s << "medicine_theory_comparative_anatomy";
                                }
                                else if (know.bits.medicine_theory_the_voice)
                                {
                                    s << "medicine_theory_the_voice";
                                }
                                else if (know.bits.medicine_theory_classification_of_muscles)
                                {
                                    s << "medicine_theory_classification_of_muscles";
                                }
                                else if (know.bits.medicine_theory_classification_of_mental_illnesses)
                                {
                                    s << "medicine_theory_classification_of_mental_illnesses";
                                }
                                else if (know.bits.medicine_theory_treatment_of_mental_illnesses)
                                {
                                    s << "medicine_theory_treatment_of_mental_illnesses";
                                }
                                else if (know.bits.medicine_tool_dedicated_hospitals)
                                {
                                    s << "medicine_tool_dedicated_hospitals";
                                }
                                else if (know.bits.medicine_method_professional_hospital_staff)
                                {
                                    s << "medicine_method_professional_hospital_staff";
                                }
                                else if (know.bits.medicine_method_specialized_wards)
                                {
                                    s << "medicine_method_specialized_wards";
                                }
                                else if (know.bits.medicine_method_hospital_lab)
                                {
                                    s << "medicine_method_hospital_lab";
                                }
                                else if (know.bits.medicine_method_medical_school)
                                {
                                    s << "medicine_method_medical_school";
                                }
                            }
                            break;
                        case 11:
                            {
                                df::knowledge_scholar_flags_11 know(r->knowledge_flags);
                                if (know.bits.medicine_method_asylum_for_mentally_ill)
                                {
                                    s << "medicine_method_asylum_for_mentally_ill";
                                }
                            }
                            break;
                        case 12:
                            {
                                df::knowledge_scholar_flags_12 know(r->knowledge_flags);
                                if (know.bits.engineering_horology_shadow_clock)
                                {
                                    s << "engineering_horology_shadow_clock";
                                }
                                else if (know.bits.engineering_horology_water_clock)
                                {
                                    s << "engineering_horology_water_clock";
                                }
                                else if (know.bits.engineering_horology_conical_water_clock)
                                {
                                    s << "engineering_horology_conical_water_clock";
                                }
                                else if (know.bits.engineering_horology_water_clock_reservoir)
                                {
                                    s << "engineering_horology_water_clock_reservoir";
                                }
                                else if (know.bits.engineering_horology_astrarium)
                                {
                                    s << "engineering_horology_astrarium";
                                }
                                else if (know.bits.engineering_horology_hourglass)
                                {
                                    s << "engineering_horology_hourglass";
                                }
                                else if (know.bits.engineering_horology_mechanical_clock)
                                {
                                    s << "engineering_horology_mechanical_clock";
                                }
                                else if (know.bits.engineering_machine_theory_of_pulley)
                                {
                                    s << "engineering_machine_theory_of_pulley";
                                }
                                else if (know.bits.engineering_machine_pulley)
                                {
                                    s << "engineering_machine_pulley";
                                }
                                else if (know.bits.engineering_machine_theory_of_screw)
                                {
                                    s << "engineering_machine_theory_of_screw";
                                }
                                else if (know.bits.engineering_machine_screw)
                                {
                                    s << "engineering_machine_screw";
                                }
                                else if (know.bits.engineering_machine_theory_of_wheel_and_axle)
                                {
                                    s << "engineering_machine_theory_of_wheel_and_axle";
                                }
                                else if (know.bits.engineering_machine_windlass)
                                {
                                    s << "engineering_machine_windlass";
                                }
                                else if (know.bits.engineering_machine_theory_of_wedge)
                                {
                                    s << "engineering_machine_theory_of_wedge";
                                }
                                else if (know.bits.engineering_machine_theory_of_lever)
                                {
                                    s << "engineering_machine_theory_of_lever";
                                }
                                else if (know.bits.engineering_machine_lever)
                                {
                                    s << "engineering_machine_lever";
                                }
                                else if (know.bits.engineering_machine_straight_beam_balance)
                                {
                                    s << "engineering_machine_straight_beam_balance";
                                }
                                else if (know.bits.engineering_machine_theory_of_gears)
                                {
                                    s << "engineering_machine_theory_of_gears";
                                }
                                else if (know.bits.engineering_machine_warded_lock)
                                {
                                    s << "engineering_machine_warded_lock";
                                }
                                else if (know.bits.engineering_machine_tumbler_lock)
                                {
                                    s << "engineering_machine_tumbler_lock";
                                }
                                else if (know.bits.engineering_machine_padlock)
                                {
                                    s << "engineering_machine_padlock";
                                }
                                else if (know.bits.engineering_machine_camshaft)
                                {
                                    s << "engineering_machine_camshaft";
                                }
                                else if (know.bits.engineering_machine_crankshaft)
                                {
                                    s << "engineering_machine_crankshaft";
                                }
                                else if (know.bits.engineering_machine_water_powered_sawmill)
                                {
                                    s << "engineering_machine_water_powered_sawmill";
                                }
                                else if (know.bits.engineering_machine_chariot_odometer)
                                {
                                    s << "engineering_machine_chariot_odometer";
                                }
                                else if (know.bits.engineering_machine_chain_drive)
                                {
                                    s << "engineering_machine_chain_drive";
                                }
                                else if (know.bits.engineering_machine_mechanical_compass)
                                {
                                    s << "engineering_machine_mechanical_compass";
                                }
                                else if (know.bits.engineering_machine_differential_gear)
                                {
                                    s << "engineering_machine_differential_gear";
                                }
                                else if (know.bits.engineering_machine_combination_lock)
                                {
                                    s << "engineering_machine_combination_lock";
                                }
                                else if (know.bits.engineering_machine_verge_escapement)
                                {
                                    s << "engineering_machine_verge_escapement";
                                }
                                else if (know.bits.engineering_machine_balance_wheel)
                                {
                                    s << "engineering_machine_balance_wheel";
                                }
                                else if (know.bits.engineering_fluid_theory_of_siphon)
                                {
                                    s << "engineering_fluid_theory_of_siphon";
                                }
                            }
                            break;
                        case 13:
                            {
                                df::knowledge_scholar_flags_13 know(r->knowledge_flags);
                                if (know.bits.engineering_fluid_valves)
                                {
                                    s << "engineering_fluid_valves";
                                }
                                else if (know.bits.engineering_fluid_force_pump)
                                {
                                    s << "engineering_fluid_force_pump";
                                }
                                else if (know.bits.engineering_optics_crystal_lens)
                                {
                                    s << "engineering_optics_crystal_lens";
                                }
                                else if (know.bits.engineering_optics_water_filled_spheres)
                                {
                                    s << "engineering_optics_water_filled_spheres";
                                }
                                else if (know.bits.engineering_optics_glass_lens)
                                {
                                    s << "engineering_optics_glass_lens";
                                }
                                else if (know.bits.engineering_optics_camera_obscura)
                                {
                                    s << "engineering_optics_camera_obscura";
                                }
                                else if (know.bits.engineering_optics_parabolic_mirror)
                                {
                                    s << "engineering_optics_parabolic_mirror";
                                }
                                else if (know.bits.engineering_optics_theory_of_color)
                                {
                                    s << "engineering_optics_theory_of_color";
                                }
                                else if (know.bits.engineering_optics_theory_of_rainbows)
                                {
                                    s << "engineering_optics_theory_of_rainbows";
                                }
                                else if (know.bits.engineering_optics_law_of_refraction)
                                {
                                    s << "engineering_optics_law_of_refraction";
                                }
                                else if (know.bits.engineering_design_models_and_templates)
                                {
                                    s << "engineering_design_models_and_templates";
                                }
                                else if (know.bits.engineering_construction_wood_lamination)
                                {
                                    s << "engineering_construction_wood_lamination";
                                }
                                else if (know.bits.engineering_astronomy_dioptra)
                                {
                                    s << "engineering_astronomy_dioptra";
                                }
                                else if (know.bits.engineering_astronomy_astrolabe)
                                {
                                    s << "engineering_astronomy_astrolabe";
                                }
                                else if (know.bits.engineering_astronomy_armillary_sphere)
                                {
                                    s << "engineering_astronomy_armillary_sphere";
                                }
                                else if (know.bits.engineering_astronomy_spherical_astrolabe)
                                {
                                    s << "engineering_astronomy_spherical_astrolabe";
                                }
                                else if (know.bits.engineering_astronomy_mural_instrument)
                                {
                                    s << "engineering_astronomy_mural_instrument";
                                }
                                else if (know.bits.engineering_astronomy_orrery)
                                {
                                    s << "engineering_astronomy_orrery";
                                }
                                else if (know.bits.engineering_machine_water_powered_trip_hammer)
                                {
                                    s << "engineering_machine_water_powered_trip_hammer";
                                }
                                else if (know.bits.engineering_machine_double_acting_piston_bellows)
                                {
                                    s << "engineering_machine_double_acting_piston_bellows";
                                }
                                else if (know.bits.engineering_fluid_archimedes_principle)
                                {
                                    s << "engineering_fluid_archimedes_principle";
                                }
                                else if (know.bits.engineering_optics_atmospheric_refraction)
                                {
                                    s << "engineering_optics_atmospheric_refraction";
                                }
                                else if (know.bits.engineering_optics_cause_of_twilight)
                                {
                                    s << "engineering_optics_cause_of_twilight";
                                }
                                else if (know.bits.engineering_optics_height_of_atmosphere)
                                {
                                    s << "engineering_optics_height_of_atmosphere";
                                }
                                else if (know.bits.engineering_machine_piston)
                                {
                                    s << "engineering_machine_piston";
                                }
                                else if (know.bits.engineering_machine_crank)
                                {
                                    s << "engineering_machine_crank";
                                }
                                else if (know.bits.engineering_machine_bellows)
                                {
                                    s << "engineering_machine_bellows";
                                }
                                else if (know.bits.engineering_machine_water_powered_piston_bellows)
                                {
                                    s << "engineering_machine_water_powered_piston_bellows";
                                }
                                else if (know.bits.engineering_machine_water_wheel)
                                {
                                    s << "engineering_machine_water_wheel";
                                }
                                else if (know.bits.engineering_machine_trip_hammer)
                                {
                                    s << "engineering_machine_trip_hammer";
                                }
                            }
                            break;
                    }
                    s << ".";
                }
                break;
            default:
                s << " " << ENUM_KEY_STR(general_ref_type, (*ref)->getType());
        }
    }
}

void WebLegends::render_item(std::ostream & s, int32_t id)
{
    CoreSuspender suspend;

    auto item = df::artifact_record::find(id);
    if (item == nullptr)
    {
        return;
    }

    simple_header(s, item);

    s << "<p>";
    categorize(s, item);
    if (auto crafted = virtual_cast<df::item_crafted>(item->item))
    {
        if (auto maker = df::historical_figure::find(crafted->maker))
        {
            s << " created by ";
            link(s, maker);
        }
        else if (auto race = df::creature_raw::find(crafted->maker_race))
        {
            s << " created by " << race->name[1];
        }
    }
    if (auto slab = virtual_cast<df::item_slabst>(item->item))
    {
        s << "<br>" << slab->description;
    }
    s << "</p>";
    if (auto constructed = virtual_cast<df::item_constructed>(item->item))
    {
        for (auto it = constructed->improvements.begin(); it != constructed->improvements.end(); it++)
        {
            s << "<p>";
            switch ((*it)->getType())
            {
                case improvement_type::ART_IMAGE:
                    s << "ART_IMAGE";
                    break;
                case improvement_type::COVERED:
                    s << "COVERED";
                    break;
                case improvement_type::RINGS_HANGING:
                    s << "RINGS_HANGING";
                    break;
                case improvement_type::BANDS:
                    s << "BANDS";
                    break;
                case improvement_type::SPIKES:
                    s << "SPIKES";
                    break;
                case improvement_type::ITEMSPECIFIC:
                    if (auto imp = virtual_cast<df::itemimprovement_itemspecificst>(*it))
                    {
                        switch (imp->type)
                        {
                            case itemimprovement_specific_type::HANDLE:
                                s << " It has a ";
                                material(s, item, imp);
                                s << " handle.";
                                break;
                            case itemimprovement_specific_type::ROLLERS:
                                s << " It has ";
                                material(s, item, imp);
                                s << " rollers.";
                                break;
                        }
                    }
                    break;
                case improvement_type::THREAD:
                    s << "THREAD";
                    break;
                case improvement_type::CLOTH:
                    s << "CLOTH";
                    break;
                case improvement_type::SEWN_IMAGE:
                    s << "SEWN_IMAGE";
                    break;
                case improvement_type::PAGES:
                    if (auto imp = virtual_cast<df::itemimprovement_pagesst>(*it))
                    {
                        s << "The " << ItemTypeInfo(item->item).toString() << " contains " << imp->count << " ";
                        material(s, item, imp);
                        if (imp->count == 1)
                        {
                            s << " page.";
                        }
                        else
                        {
                            s << " pages.";
                        }
                        for (auto it2 = imp->contents.begin(); it2 != imp->contents.end(); it2++)
                        {
                            if (auto content = df::written_content::find(*it2))
                            {
                                written_content(s, item, content);
                            }
                        }
                    }
                    break;
                case improvement_type::ILLUSTRATION:
                    s << "ILLUSTRATION";
                    break;
                case improvement_type::INSTRUMENT_PIECE:
                    s << "INSTRUMENT_PIECE";
                    break;
                case improvement_type::WRITING:
                    if (auto imp = virtual_cast<df::itemimprovement_writingst>(*it))
                    {
                        for (auto it2 = imp->anon_1.begin(); it2 != imp->anon_1.end(); it2++)
                        {
                            if (auto content = df::written_content::find(*it2))
                            {
                                written_content(s, item, content);
                            }
                        }
                    }
                    break;
            }
            s << "</p>";
        }
    }

    history(s, item);
    // TODO
    s << "</body></html>";
}