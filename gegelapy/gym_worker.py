import sys, gymnasium as gym, struct, pickle

def main():
    # Load environment configuration
    size_raw = sys.stdin.buffer.read(4)
    size = struct.unpack('I', size_raw)[0]
    payload = pickle.loads(sys.stdin.buffer.read(size))
    
    env = payload['callback'](*payload['args'], **payload['kwargs'])

    while True:
        cmd_raw = sys.stdin.buffer.read(1)
        if not cmd_raw: break
        cmd = cmd_raw.decode('ascii')
        
        if cmd == 'r':
            seed = struct.unpack('Q', sys.stdin.buffer.read(8))[0]
            o, _ = env.reset(seed=seed)
            obs = o.flatten()
            sys.stdout.buffer.write(struct.pack('I', len(obs)) + obs.astype('float64').tobytes())
        elif cmd == 's':
            nb_actions = struct.unpack('I', sys.stdin.buffer.read(4))[0]
            actions_raw = sys.stdin.buffer.read(nb_actions * 8)
            actions = list(struct.unpack(f'{nb_actions}d', actions_raw))
            if isinstance(env.action_space, gym.spaces.Discrete):
                # Discrete expects a single integer (usually actions[0])
                action_to_send = int(actions[0])
            elif isinstance(env.action_space, gym.spaces.Box):
                # Box (Continuous) expects a numpy array or list of floats
                import numpy as np
                action_to_send = np.array(actions, dtype=np.float32)
            elif isinstance(env.action_space, gym.spaces.MultiDiscrete):
                # MultiDiscrete expects a list/array of integers
                action_to_send = [int(a) for a in actions]
            else:
                # Fallback: send as is
                action_to_send = actions
            o, r, t, tr, _ = env.step(action_to_send)
            obs = o.flatten()
            header = struct.pack('d?I', r, (t or tr), len(obs))
            sys.stdout.buffer.write(header + obs.astype('float64').tobytes())
        sys.stdout.buffer.flush()

if __name__ == "__main__":
    main()
