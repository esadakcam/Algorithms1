/*
*   Mehmed Esad AKCAM   
*   150190725
*/


#include <iostream>
#include <vector>
#include <fstream>
#include <time.h>
#include <stdio.h>
#include "Tweet.h"
using namespace std;

void swap(vector<Tweet> &t, int first, int second);                    // swap opertion for partition function
void deterministicQuicksort(vector<Tweet> &tweets, int low, int high); //deterministic quick sort signature
int determinsticPartition(vector<Tweet> &tweets, int low, int high);   //partition for deterministic quicksort
void randomizedQuicksort(vector<Tweet> &tweets, int low, int high);    //randomized quicksort signature
int randomizedPartition(vector<Tweet> &tweets, int low, int high);     //partition for randomized quicksort

void readfile(vector<Tweet> &tweetArray, char location[100], int n);  //function for read tweets file
void writefile(vector<Tweet> &tweetArray, char location[100], int n); //function for write sorted tweets

int main(int argc, char *argv[])
{
    int N; //num of tweets

    sscanf(argv[1], "%d", &N);
    vector<Tweet> tweetArray;
    readfile(tweetArray, argv[3], N);

    clock_t t;             // to measuere elapsed time
    if (argv[2][0] == 'r') // randomized or deterministic
    {
        t = clock();
        randomizedQuicksort(tweetArray, 0, N - 1);
    }
    else
    {
        t = clock();
        deterministicQuicksort(tweetArray, 0, N - 1);
    }

    t = clock() - t;
    cout << "Sorted in " << ((float)t / CLOCKS_PER_SEC) * 1000 << " miliseconds by using " << argv[2] << " pivot selection." << endl;
    writefile(tweetArray, argv[4], N);
    return 0;
}

void randomizedQuicksort(vector<Tweet> &tweets, int low, int high)
{
    if (low < high)
    {
        /*partition the array like [L,x,R] where L contains elements that are less than the pivot and 
        R subarray contains elements that are grater than the pivot*/
        int pivotIndex = randomizedPartition(tweets, low, high);
        randomizedQuicksort(tweets, low, pivotIndex - 1);  // quicksort for elements that are less than the pivot
        randomizedQuicksort(tweets, pivotIndex + 1, high); // quicksort for elements that are grater than the pivot
    }
}

void deterministicQuicksort(vector<Tweet> &tweets, int low, int high)
{
    if (low < high)
    {
        /*partition the array like [L,x,R] where L contains elements that are less than the pivot and 
        R subarray contains elements that are grater than the pivot*/
        int pivotIndex = determinsticPartition(tweets, low, high);

        deterministicQuicksort(tweets, low, pivotIndex - 1);  // quicksort for elements that are less than the pivot
        deterministicQuicksort(tweets, pivotIndex + 1, high); // quicksort for elements that are grater than the pivot
    }
}

int determinsticPartition(vector<Tweet> &tweets, int low, int high)
{
    /*
    * Itarete over all elements, if an element is less than the pivot, slide pivot one place and swap the element 
    *  with the element that is laying on the pivot's place. After iteration insert the pivot into its correct position.
    */

    Tweet pivot = tweets[high]; //always select the last element of array as the pivot
    int i = low - 1;            //pivot's place after the partition
    for (int k = low; k < high; k++)
    {
        //swap each pass smaller element to left of the pivot and greater element to right of the pivot
        if (tweets[k].country < pivot.country)
        {
            i++; //slide pivot's actual place to the right
            swap(tweets, i, k);
        }
        else if (tweets[k].country == pivot.country)
        {
            if (tweets[k].artist_name < pivot.artist_name)
            {
                i++; //slide pivot's actual place to the right
                swap(tweets, i, k);
            }
            else if (tweets[k].artist_name == pivot.artist_name)
            {
                if (tweets[k].tweet_unixtime < pivot.tweet_unixtime)
                {
                    i++; //slide pivot's actual place to the right
                    swap(tweets, i, k);
                }
            }
        }
    }
    swap(tweets, i + 1, high); //insert pivot to its correct position
    return i + 1;
}

int randomizedPartition(vector<Tweet> &tweets, int low, int high)
{
    srand(time(NULL));

    int pivotIndex = rand() % (high - low); //select pivot randomly
    pivotIndex += low;
    swap(tweets, pivotIndex, high);                  //insert the pivot at the end of the array
    return determinsticPartition(tweets, low, high); //apply deterministic partition
}

void swap(vector<Tweet> &t, int first, int second)
{
    Tweet temp = t[first]; //temporary element not to loose the first element data during exchanging
    t[first] = t[second];
    t[second] = temp;
}

//reads first n tweet from file push each tweet into tweetArray
void readfile(vector<Tweet> &tweetArray, char location[100], int n)
{
    string read;
    ifstream tweet_file(location); //open the file
    bool flag = false;             //to skip the first line
    if (tweet_file.is_open())
    {
        int attributeIndex = 0;
        string attribute = "";
        string tweet_id;
        string tweet_datetime;
        string tweet_unixtime;
        string artist_name;
        string track_title;
        string country;
        int index = 0;

        while (getline(tweet_file, read)) // read line by line
        {
            if (index >= n) //if the goal is reached stop the read
            {
                break;
            }
            tweet_id = "";
            tweet_datetime = "";
            tweet_unixtime = "";
            artist_name = "";
            track_title = "";
            country = "";
            if (flag)
            {
                char c;
                for (int i = 0; i < read.size(); i++) //read ith tweet
                {
                    c = read[i];
                    if (c == '\t') //if character c is tab char, an attribute is parsed. assign it to corresponding field.
                    {
                        switch (attributeIndex)
                        {
                        case 0:
                            tweet_id = attribute;
                            break;
                        case 1:
                            tweet_datetime = attribute;
                            break;
                        case 2:
                            tweet_unixtime = attribute;
                            break;
                        case 3:
                            artist_name = attribute;
                            break;
                        case 4:
                            track_title = attribute;
                            break;
                        case 5:
                            country = attribute;
                            break;
                        }
                        attribute = "";   //clear attribute
                        attributeIndex++; //increase attribute index by one
                    }
                    else
                    {
                        attribute += c; //append attribute
                    }
                }
                country = attribute;
                attributeIndex = 0;
                attribute = "";
            }
            if (flag)
            {
                tweetArray.push_back(Tweet(tweet_id, tweet_datetime, tweet_unixtime,
                                           artist_name, track_title, country)); //push ith tweet into array
                index++;
            }
            flag = true;
        }
    }
}

void writefile(vector<Tweet> &tweetArray, char location[100], int n)
{
    ofstream file(location); //open file
    int index = 0;
    file << "tweet_id\ttweet_datetime\ttweet_unixtime\tartist_name\ttrack_title\tcountry\n";
    for (int i = 0; i < tweetArray.size(); i++)
    {
        Tweet tweet = tweetArray[i];
        //write ith tweet into file
        file << tweet.tweet_id << '\t' << tweet.tweet_datetime_original << '\t' << tweet.tweet_unixtime << '\t' << tweet.artist_name_original
             << '\t' << tweet.track_title_original << '\t' << tweet.country_original << '\n';
        index++;
        if (index >= n)
        {
            break;
        }
    }
    file.close();
}