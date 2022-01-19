using namespace std;

//tweet class to represent each tweet
class Tweet
{
    string to_lower(string in); //since the comparisons are case insensitive convert strings to lower
public:
    //original fields are used to write the sorted array to file
    //fileds that has not original posfix are used to comparisons and converted into lower
    string tweet_id;

    string tweet_datetime;
    string tweet_datetime_original;

    string tweet_unixtime;

    string artist_name;
    string artist_name_original;

    string track_title;
    string track_title_original;

    string country;
    string country_original;

    Tweet();
    Tweet(string tweet_id, string tweet_datetime, string tweet_unixtime, // constructor
          string artist_name, string track_title, string country);
};

string Tweet::to_lower(string in)
{
    string ret = "";
    for (int i = 0; i < in.size(); i++)
    {
        if (in[i] >= 65 && in[i] <= 90) // if ith character is not lower convert it to lower
        {
            ret += in[i] + 32;
        }
        else
        {
            ret += in[i];
        }
    }
    return ret;
}
Tweet::Tweet(string tweet_id, string tweet_datetime, string tweet_unixtime,
             string artist_name, string track_title, string country)
{
    this->artist_name = to_lower(artist_name);
    this->artist_name_original = artist_name;

    this->tweet_id = tweet_id;
    this->track_title = to_lower(track_title);
    this->track_title_original = track_title;

    this->tweet_datetime = to_lower(tweet_datetime);
    this->tweet_datetime_original = tweet_datetime;

    this->country = to_lower(country);
    this->country_original = country;

    this->tweet_unixtime = tweet_unixtime;
}