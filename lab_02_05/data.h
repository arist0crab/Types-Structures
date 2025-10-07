#ifndef __DATA_H__
#define __DATA_H__

#define MAX_STR_LEN 128
#define MAX_PLAYS_QUANTITY 1000

typedef enum { PIECE, MUSICAL } play_type_t;
typedef enum { BALLET, OPERA, MUSICAL_SHOW } musical_genre_t;
typedef enum { DRAMA, COMEDY, FAIRY_TALE } piece_genre_t;
typedef enum { AGE_3PLUS, AGE_10PLUS, AGE_16PLUS, AGE_RATING_NONE } age_rating_t;

typedef enum 
{
    EXIT_THE_PROGRAM,
    READ_FILE,
    PRINT_DATA,
    PRINT_KEY_TABLE,
    ADD_THEATER_PLAY,
    DELETE_THEATER_PLAY,
    PRICE_QUICK_SORT,
    PRICE_SLOW_SORT,
    KEY_TABLE_QUICK_SORT,
    KEY_TABLE_SLOW_SORT,
    PRINT_DATA_KEY_TABLE,
    PRINT_EFFICIENCY_TABLE,
    PRINT_BALETS,

    CHOICES_QUANTITY
} choice_t;

typedef struct
{
    char theater_name[MAX_STR_LEN];
    char play_name[MAX_STR_LEN];
    double ticket_price;
    double max_ticket_price;
    play_type_t play_type;
    age_rating_t age_rating;
    union 
    {
        struct 
        {
            piece_genre_t piece_genre;            
        } piece_info;
        struct 
        {
            char composer[MAX_STR_LEN];
            char country[MAX_STR_LEN];
            musical_genre_t musical_genre;
            int duration;
        } musical_info;
    } play_data;
} theater_play_t;

#endif