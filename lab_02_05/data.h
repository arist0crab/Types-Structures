#ifndef __DATA_H__
#define __DATA_H__

#define MAX_STR_LEN 128

typedef struct
{
    char theater_name[MAX_STR_LEN];
    char play_name[MAX_STR_LEN];
    double ticket_price;
    double max_ticket_price;
    play_type_t play_type;
    union 
    {
        struct 
        {
            piece_genre_t piece_genre;
            age_rating_t age_rating;
        } piece_info;
        struct 
        {
            char composer[MAX_STR_LEN];
            char country[MAX_STR_LEN];
            musical_genre_t musical_genre;
            age_rating_t age_rating;
            int duration;
        } musical_info;
        
    };
    
} theater_play_t;

typedef enum 
{
    PIECE,
    MUSICAL
} play_type_t;

typedef enum 
{
    BALLET,
    OPERA,
    MUSICAL_SHOW
} musical_genre_t;

typedef enum 
{
    DRAMA, 
    COMEDY,
    FAIRY_TALE
} piece_genre_t;

typedef enum
{
    AGE_3PLUS,
    AGE_10PLUS,
    AGE_16PLUS
} age_rating_t;

#endif