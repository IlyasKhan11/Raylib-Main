import sqlite3

# Function to fetch all table names
def fetch_table_names(cursor):
    cursor.execute("SELECT name FROM sqlite_master WHERE type='table';")
    return [row[0] for row in cursor.fetchall()]

# Function to fetch and print all users from the 'users' table
def fetch_all_users(database_path):
    try:
        conn = sqlite3.connect(database_path)
        cursor = conn.cursor()
        
        # List all tables
        tables = fetch_table_names(cursor)
        if "users" not in tables:
            print("No 'users' table found in the database.")
            return
        
        # Fetch all users
        cursor.execute("SELECT * FROM users;")
        users = cursor.fetchall()
        
        # Print user details
        if users:
            print("Users in the database:")
            for user in users:
                print(user)
        else:
            print("No users found in the 'users' table.")
    
    except sqlite3.Error as e:
        print(f"SQLite error: {e}")
    finally:
        if conn:
            conn.close()

# Example usage
database_path = "users.db"  # Path to your SQLite database
fetch_all_users(database_path)
