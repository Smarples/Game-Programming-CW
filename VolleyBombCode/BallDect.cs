using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class BallDect : MonoBehaviour {
//Canvas contains the text for score and bomb timer
public GameObject UICanvas;
//bomb prefab contains the prefab for the bomb which contains this script and bounciness
public GameObject BombPrefab;

//balltime is the time the bomb takes to explode in seconds
public float ballTime = 30;

//public counttexts and scoretext so that the textboxes could be applied to the variables
public Text countText;
public Text countText2;
private int count;
private int count2;

public int scoreText;

// Use this for initialization
void Start ()
{
    // sets the score counts to 0 on start
    count = 0;
    SetCountText();


}

//method that would have been used to update the score per point
void UpdateScore()
{
    
}

//updates the UI with the scores
void SetCountText(){

    countText.text = "Player 1 : " + count.ToString();
    countText2.text = "Player 2 : " + count2.ToString();

}


//used to detect collisions
void OnCollisionEnter (Collision col){
    //if the ball collides with the court
	if(col.gameObject.name == "Court")
	{
        //deletes the bomb, uses a prefab to spawn a new bomb and place it in the spawn position
        GameObject cloneBomb = Instantiate(BombPrefab, new Vector3(-8,18,-1), Quaternion.identity);
        cloneBomb.transform.localScale = new Vector3(4,4,4);
        Destroy(gameObject);

        //increases the score count by 1 (would have added more code to detect which side of the court the ball landed on to add points appropriately)
        count = count + 1;
        count2 = count2 + 1;
        SetCountText();

    }

    //if the bomb collides with the plane placed outside the court then a new bomb is spawned and a point is awarded appropriately
    if (col.gameObject.name == "Plane"){
        GameObject cloneBomb = Instantiate(BombPrefab, new Vector3(-8,18,-1), Quaternion.identity);
        cloneBomb.transform.localScale = new Vector3(4,4,4);
        Destroy(gameObject);
        //test code to award a point if the ball landed beyond a certain x coordinate
        if (transform.position.x > 0){
            count2 = count2 + 1;
        }

    
        
} 

}
}