/**
 * @file commands.h
 * @brief Déclaration des commandes internes de l'interpréteur ScriptBasic-ng
 * 
 * Standard C23 strict. Fichier plat maintenu manuellement (pas de génération magique).
 * Voir AGENTS.md pour les règles de modification.
 */

#ifndef SCRIPTBASIC_COMMANDS_H
#define SCRIPTBASIC_COMMANDS_H

#include <stdint.h>
#include <stdbool.h>

// Préfixe pour éviter les collisions de symboles (visibilité cachée par défaut via -fvisibility=hidden)
#define SB_CMD_PREFIX sb_cmd_

// Types de base explicites (C23)
typedef int32_t sb_int_t;
typedef uint32_t sb_uint_t;
typedef double sb_real_t;
typedef bool sb_bool_t;

// Structure de contexte d'exécution (squelette)
typedef struct sb_execute_t {
    sb_int_t error_code;
    sb_bool_t is_running;
    // ... autres champs d'état
} sb_execute_t;

// Déclaration des fonctions de commande (exemples)
sb_int_t SB_CMD_PREFIX_let(sb_execute_t *pExecute, sb_int_t var_idx, sb_real_t value);
sb_int_t SB_CMD_PREFIX_if(sb_execute_t *pExecute, sb_bool_t condition);
sb_int_t SB_CMD_PREFIX_exitfor(sb_execute_t *pExecute);

#endif // SCRIPTBASIC_COMMANDS_H