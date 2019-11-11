using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class BossAI : MonoBehaviour
{
    public float damage = .1f;
    public float maxSpeed = 1f;
    public float fireRate = 1.0f;
    public float canFire = 0.0f;
    public int MaxDist = 10;
    public int MinDist = 5;

    public float maxEnemyHealth;
    public float enemyShield;

    [SerializeField]
    int points;
    [SerializeField]
    GameObject powerUp;
    [SerializeField]
    float timeCount = 2;

    public GameObject enemyExplosion;

    public bool playerInRange = false;
    public bool playerBehind = false;
    private bool bossDestroyed = false;


    private GameManager gameManager;
    private Transform player;
    private float enemyHealth;


    void Start()
    {
        gameManager = GameObject.FindGameObjectWithTag("GameManager").GetComponent<GameManager>();
        enemyHealth = maxEnemyHealth;
    }

    // Update is called once per frame
    void Update()
    {
         Movement();
        if (playerBehind)
        {
            LookAtPlayer();
        }
    }

    private void OnTriggerEnter(Collider other)
    {
        if (other.gameObject.tag == "Player")
        {
            if (this.tag != "PlayerWeapon")
            {
                other.gameObject.GetComponent<PlayerStats>().takeDamage(damage);

                var magnitude = 1500;

                var force = transform.position - other.transform.position;

                force.Normalize();
                other.GetComponent<Rigidbody>().AddForce(-force * magnitude);
            }
        }
    }

    public void takeDamageAI(float damage)
    {
        float remainingDamage;

        if (damage > enemyShield && enemyShield != 0)
        {
            remainingDamage = damage - enemyShield;// Get damage remaining after shields are gone
            enemyShield = 0;// Set shields to 0
            enemyHealth = Mathf.Clamp(enemyHealth - remainingDamage, 0, maxEnemyHealth);// Put remaining damage to health 
        }
        else if (enemyShield == 0)
        {
            enemyHealth = Mathf.Clamp(enemyHealth - damage, 0, maxEnemyHealth);// Damage health 
        }
        else
        {
            enemyShield = Mathf.Clamp(enemyShield - damage, 0, maxEnemyHealth); //Damage Shields
        }
        if (enemyHealth == 0)
        {
            DestroyAI();
        }
    }
    void DestroyAI()
    {
        if (!bossDestroyed)
        {
            bossDestroyed = true;
            Instantiate(enemyExplosion, transform.position, Quaternion.identity);
            Instantiate(powerUp, transform.position, Quaternion.identity);

            gameManager.BossDestroyed();
            gameManager.addPoints(points);
            Destroy(gameObject);
        }
    }

    public void Movement()
    {
        if (player == null)
        {
            // Find the player's ship!
            GameObject go = GameObject.FindWithTag("Player");

            if (go != null)
            {
                player = go.transform;
            }
        }

        // At this point, we've either found the player,
        // or he/she doesn't exist right now.

        if (player == null)
            return; // Try again next frame!

        // HERE -- we know for sure we have a player. Turn to face it!
        if (!playerInRange)
        {
            LookAtPlayer();

            //move
            if (Vector3.Distance(transform.position, player.position) >= MinDist)
            {

                transform.position += transform.forward * maxSpeed * Time.deltaTime;

            }
        }
        else
        {
             float dirNum;
             Vector3 heading = player.position - transform.position;// Get heading direction
             dirNum = AngleDir(transform.forward, heading, transform.up);// Call function AngleDir and determine side of ship

            if(dirNum == -1)// If AngleDir returns negative one circle player left
            {
                CirclePlayerLeft();
            }
            else if(dirNum == 1)// else if one circle right
            {
                CirclePlayerRight();
            }
        }
    }

    float AngleDir(Vector3 fwd, Vector3 targetDir, Vector3 up)
    {
        Vector3 perp = Vector3.Cross(fwd, targetDir);
        float dir = Vector3.Dot(perp, up);

        if (dir > 0f)
        {
            return 1f;
        }
        else if (dir < 0f)
        {
            return -1f;
        }
        else
        {
            return 0f;
        }
    }

    void LookAtPlayer()
    {
        //Get player direction and put to Quaternion
        var dir = player.transform.position - transform.position;
        var angle = Mathf.Atan2(dir.x, dir.z) * Mathf.Rad2Deg;
        Quaternion direction = Quaternion.AngleAxis(angle, Vector3.up);
        //Rotate to face player
        transform.rotation = Quaternion.Slerp(transform.rotation, direction, timeCount * Time.deltaTime);
    }
    void CirclePlayerLeft()
    {
        // Rotate around player
        transform.RotateAround(player.position, Vector3.down, 15 * Time.deltaTime);
        //Get direction of player
        var dir = player.transform.position - transform.position;
        var angle = Mathf.Atan2(dir.x, dir.z) * Mathf.Rad2Deg;
        Quaternion direction = Quaternion.AngleAxis(angle + 90, Vector3.up);// Store direction so left side ship faces player

        transform.rotation = Quaternion.Slerp(transform.rotation, direction, timeCount * Time.deltaTime);// Rotate ship to have left side face player
    }
    void CirclePlayerRight()
    {
        //Rotate around player
        transform.RotateAround(player.position, Vector3.up, 15 * Time.deltaTime);
        //Get direction of player
        var dir = player.transform.position - transform.position;
        var angle = Mathf.Atan2(dir.x, dir.z) * Mathf.Rad2Deg;
        //Store direction so right side ship faces player
        Quaternion direction = Quaternion.AngleAxis(angle - 90, Vector3.up);
        // Rotate right side to face player.
        transform.rotation = Quaternion.Slerp(transform.rotation, direction, timeCount * Time.deltaTime);
    }

    public IEnumerator PlayerFoundBehind()
    {
        //If player is found behind ship set player behind true for three seconds
        playerBehind = true;
        yield return new WaitForSeconds(3.0f);
        playerBehind = false;
    }
}
