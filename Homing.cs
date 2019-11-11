using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Homing : MonoBehaviour
{
    private bool playerFound = false;
    private GameObject player = null;
    public float speed = 2.0f;
    public float timeCount = 1f;
    [SerializeField]
    private bool isMoving = false;
    [SerializeField]
    float timeToMove = 1;
    [SerializeField]
    private GameObject exhaust;
    private bool isPaused = false;


    void Update()
    {
        if (!isPaused)
        {
            if (playerFound)
            {
                var dir = player.transform.position - transform.position;
                var angle = Mathf.Atan2(dir.x, dir.z) * Mathf.Rad2Deg;
                Quaternion direction = Quaternion.AngleAxis(angle, Vector3.up);

                transform.parent.rotation = Quaternion.Slerp(transform.rotation, direction, timeCount * Time.deltaTime);
                transform.parent.Translate(Vector3.forward * speed * Time.deltaTime);
                exhaust.SetActive(true);
            }
            else if (isMoving)
            {
                transform.parent.Translate(Vector3.forward * speed * Time.deltaTime);
                exhaust.SetActive(true);
            }
            else if (Time.time < timeToMove && !isMoving)
            {
                transform.parent.Translate(Vector3.forward * (speed * 3) * Time.deltaTime);
                exhaust.SetActive(true);
            }
            else exhaust.SetActive(false);
        }

        if (transform.position.z >= 95)
        {
            Destroy(this.gameObject);

        }

        if (transform.position.z <= -95)
        {
            Destroy(this.gameObject);
        }

        if (transform.position.x >= 95)
        {
            Destroy(this.gameObject);

        }

        if (transform.position.x <= -95)
        {
            Destroy(this.gameObject);
        }
    }

    void OnEnable()
    {
        GameManager.onClickedPause += pause;
    }
    void OnDisable()
    {
        GameManager.onClickedPause -= pause;
    }

    void pause()
    {
        if (!isPaused)
        {
            isPaused = true;
        }
        else isPaused = false;
    }

    void OnTriggerEnter(Collider other)
    {
        if (other.tag == "Player")
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
}
