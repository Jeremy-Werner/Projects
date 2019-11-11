using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class TurretScript : MonoBehaviour
{
    private bool playerFound = false;
    private float canFire = 0.0f;
    [SerializeField]
    private float fireRate = .25f;
    private GameObject player;
    [SerializeField]
    GameObject laserPrefab;

    void Update()
    {
        if (playerFound)
        {
            if (Time.time > canFire)
            {
                // fire laser when player found and time can fire
                spawnLaser();
                canFire = Time.time + fireRate;
            }
        }
    }

   void OnTriggerEnter(Collider other)
    {
        if(other.tag == "Player")
        {
            // Set player found to true and grap player object
            playerFound = true;
            player = other.gameObject;
        }
    }
    void OnTriggerExit(Collider other)
    {
        if (other.tag == "Player")
        {
            //Set player found to false
            playerFound = false;
        }
    }

    void spawnLaser()
    {
        //Spawn laser in direction of player
            var dir = player.transform.position - transform.position;
            var angle = Mathf.Atan2(dir.x, dir.z) * Mathf.Rad2Deg;
            Quaternion direction = Quaternion.AngleAxis(angle, Vector3.up);

            Instantiate(laserPrefab, transform.position, direction);
    }
}
