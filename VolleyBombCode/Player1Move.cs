 using System.Collections;
using System.Collections.Generic;
using UnityEngine;

//script for controlling player 1
public class Player1Move : MonoBehaviour {

    //public variables for editing in the unity editor
    //speed is the movement speed of the player
	public float speed = 20f;
    //jumpforce and gravity control height and speed of jump
    public float jumpForce = 20F;
    public float gravity = 25f;

	private Vector3 moveDir = Vector3.zero;


    // Use this for initialization
    void Start () {
		
	}
	
	// Update is called once per frame
	void Update () {

        //code for movement and jumping of player
        CharacterController controller = gameObject.GetComponent<CharacterController> ();

        //only allows movement of character on ground
		if (controller.isGrounded){
            //get axis used so players can change the buttons used to move horizontally or vertically
			moveDir = new Vector3 (Input.GetAxis ("Horizontal"), 0, Input.GetAxis ("Vertical"));
			moveDir = transform.TransformDirection (moveDir);
			moveDir *= speed;
            //jumps when the user presses the jump button by using the jumpforce
			if (Input.GetButtonDown ("Jump")) {
				moveDir.y = jumpForce;
			}
		}
		//applies previous if statements to change movement
		moveDir.y -= gravity * Time.deltaTime;
		controller.Move (moveDir * Time.deltaTime);
}
}